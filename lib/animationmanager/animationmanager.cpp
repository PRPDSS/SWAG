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
        facePart.resolution = {facePartJson["width"], facePartJson["height"]};

        // Извлекаем кадры
        JsonArray framesArray = facePartJson["frames"].as<JsonArray>();
        for (JsonArray frameJson : framesArray)
        {
            Frame frame;
            for (uint32_t pixel : frameJson)
            {
                frame.pixels.push_back(pixel);
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
    // Создаем документ для сериализации
    JsonDocument doc; // Увеличь размер, если данных много

    // Записываем размер массива
    doc["size"] = faceParts.size();

    // Создаем массив faceParts
    JsonArray facePartsArray = doc.createNestedArray("faceParts");

    for (const auto &facePart : faceParts)
    {
        JsonObject facePartJson = facePartsArray.createNestedObject();

        // Сохраняем размеры
        facePartJson["width"] = facePart.resolution.second;
        facePartJson["height"] = facePart.resolution.first;

        // Сохраняем кадры
        JsonArray framesArray = facePartJson.createNestedArray("frames");
        for (const auto &frame : facePart.frames)
        {
            JsonArray frameArray = framesArray.createNestedArray();
            for (const auto &pixel : frame.pixels)
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

    // Сериализуем JSON в файл
    if (serializeJson(doc, file) == 0)
    {
        Serial.println("Ошибка: не удалось записать JSON в файл!");
        file.close();
        return false;
    }

    file.close();
    return true;
}

void AnimationManager::overlayFrameOntoBuffer(const Frame &frame, uint8_t *buffer,
                                              const std::pair<uint16_t, uint16_t> &frameDimensions,
                                              const std::pair<uint16_t, uint16_t> &bufferDimensions,
                                              std::pair<uint16_t, uint16_t> &position)
{
    
}
