#include <SPIFFS.h>
#include <animationmanager.h>

#define FORMAT_SPIFFS_IF_FAILED true

// Перед использованием необходимо инициализировать SPIFFS
void setup()
{
    Serial.begin(115200);
    if (!SPIFFS.begin(true))
    {
        Serial.println("Ошибка инициализации SPIFFS!");
        return;
    }

    AnimationManager animationManager;

    // Пример данных
    std::vector<FacePart> faceParts = {
        {"eye", {{std::vector<uint32_t>{0, 1, 0, 1, 0}}, {std::vector<uint32_t>{1, 0, 1, 0, 1}}}, {0, 0}, {1, 5}},
        {"mouth", {{std::vector<uint32_t>{255, 128, 0}}, {std::vector<uint32_t>{0, 128, 255}}}, {5, 5}, {1, 3}}};

    // Сохраняем анимации в JSON файл
    if (animationManager.saveAnimationsToJson("/faceParts.json", faceParts))
    {
        Serial.println("Анимации успешно сохранены!");
    }
    else
    {
        Serial.println("Ошибка при сохранении анимаций.");
    }
}

void loop(){

}