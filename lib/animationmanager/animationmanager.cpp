#include <animationmanager.h>
#include <fstream>
#include <ArduinoJson.h>
#include <SPIFFS.h>

bool AnimationManager::loadAnimationsFromJson(const std::string &filePath, std::vector<FacePart> &faceParts)
{
    // Открываем JSON файл
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        Serial.println("Ошибка: не удалось открыть файл!");
        return false;
    }

    // Создаем динамический документ для десериализации
    JsonDocument doc;

    // Десериализуем JSON из файла
    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.println("Ошибка десериализации JSON:");
        Serial.println(error.c_str());
        return false;
    }

    // Извлекаем размер списка
    uint16_t size = doc["size"];
    JsonArray facePartsArray = doc["faceParts"].as<JsonArray>();

    // Проверяем, что количество элементов совпадает
    if (facePartsArray.size() != size)
    {
        Serial.println("Ошибка: размер массива faceParts не совпадает с 'size'!");
        return false;
    }

    // Обрабатываем каждый элемент массива faceParts
    for (JsonObject facePartJson : facePartsArray)
    {
        FacePart facePart;

        // Извлекаем разрешение
        facePart.resolution = {facePartJson["height"], facePartJson["width"]};

        // Извлекаем кадры
        JsonArray framesArray = facePartJson["frames"].as<JsonArray>();
        for (JsonArray frameJson : framesArray)
        {
            std::vector<uint32_t> frame;
            for (uint32_t pixel : frameJson)
            {
                frame.push_back(pixel);
            }
            facePart.frames.push_back(frame);
        }

        // Добавляем facePart в общий список
        faceParts.push_back(facePart);
    }

    return true;
}

bool AnimationManager::saveAnimationsToJson(const std::string &filePath, const std::vector<FacePart> &faceParts)
{

    JsonDocument doc;

    // Записываем размер массива
    doc["size"] = faceParts.size();

    JsonArray facePartsArray = doc["faceParts"].to<JsonArray>();

    for (const auto &facePart : faceParts)
    {
        JsonObject facePartJson = facePartsArray.add<JsonObject>();

        // Сохраняем размеры
        facePartJson["width"] = facePart.resolution.first;
        facePartJson["height"] = facePart.resolution.second;

        // Сохраняем кадры
        JsonArray framesArray = facePartJson["frames"].to<JsonArray>();
        for (const auto &frame : facePart.frames)
        {
            JsonArray frameArray = framesArray.add<JsonArray>();
            for (const auto &pixel : frame)
            {
                frameArray.add(pixel);
            }
        }
    }

    // Открываем файл для записи
    File file = SPIFFS.open(filePath.c_str(), FILE_WRITE);
    if (!file)
    {
        Serial.println("Ошибка: не удалось открыть файл для записи!");
        return false;
    }

    if (serializeJson(doc, file) == 0)
    {
        Serial.println("Ошибка: не удалось записать JSON в файл!");
        file.close();
        return false;
    }
    serializeJson(doc, Serial);
    file.close();
    return true;
}

void AnimationManager::overlayFrameOntoBuffer(const std::vector<uint32_t> &frame, uint32_t *buffer,
                                              const std::pair<uint16_t, uint16_t> &frameDimensions,
                                              const std::pair<uint16_t, uint16_t> &bufferDimensions,
                                              std::pair<uint16_t, uint16_t> &position)
{

    uint16_t max_x = std::max(position.first + frameDimensions.first, (int)bufferDimensions.first),
             max_y = std::max(position.second + frameDimensions.second, (int)bufferDimensions.second);
    for (int y = 0; y + position.second < max_y; y++)
    {
        for (int x = 0; x + position.first < max_x; x++)
        {
            size_t buffer_index = position.first + x + (position.second + y) * frameDimensions.first,
                   frame_index = x + y * frameDimensions.first;
            buffer[buffer_index] = blendColor(frame[frame_index], buffer[buffer_index]);
        }
    }
}

void AnimationManager::renderToBuffer(uint32_t *buffer, const std::pair<uint16_t, uint16_t> &bufferDimensions)
{
    // Очистка буфера
    for (size_t i = 0; i < bufferDimensions.first * bufferDimensions.second; ++i)
    {
        buffer[i] = 0;
    }

    // Проход по всем частям лица
    for (auto &facePart : faceParts)
    {
        // Если у части нет кадров, пропускаем её
        if (facePart.frames.empty())
            continue;

        // Получаем текущий кадр
        const auto &frame = facePart.frames[facePart.currentFrameIndex];

        // Накладываем кадр на общий буфер
        overlayFrameOntoBuffer(
            frame,               // Текущий кадр части
            buffer,              // Общий буфер
            facePart.resolution, // Размер кадра
            bufferDimensions,    // Размер буфера
            facePart.position    // Позиция части на общем буфере
        );

        // Обновляем текущий кадр (с зацикливанием)
        facePart.currentFrameIndex = (facePart.currentFrameIndex + 1) % facePart.frames.size();
    }
}

const uint32_t AnimationManager::blendColor(const uint32_t &colorA, const uint32_t &colorB)
{
    // HEX: AARRGGBB
    if (colorA && 0xFF000000 == 0) // проверка на альфа-канал
        return colorB;
    if (colorB && 0xFF000000 == 0)
        return colorA;
    double alphaA = (colorA >> 24) / 255.0f,
           alphaB = (colorB >> 24) / 255.0f;
    uint8_t r, g, b, a;
    r = ((colorA && 0xFF0000) >> 16) * alphaA + ((colorB && 0xFF0000) >> 16) * alphaB / 2;
    g = ((colorA && 0xFF00) >> 8) * alphaA + ((colorB && 0xFF00) >> 8) * alphaB / 2;
    b = (colorA && 0xFF) * alphaA + (colorB && 0xFF) * alphaB / 2;
    a = (colorA >> 24 + colorB >> 24) >> 1;
    return a << 24 + r << 16 + g << 8 + b;
}
