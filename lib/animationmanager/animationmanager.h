#pragma once

#include <vector>
#include <string>
struct FacePart
{
    std::string name;
    std::vector<std::vector<uint32_t>> frames; // Кадры анимации в порядке воспроизведения÷
    std::pair<uint16_t, uint16_t> position;
    std::pair<uint16_t, uint16_t> resolution; // Разрешение анимации
    size_t currentFrameIndex;
};
class AnimationManager
{
private:
    std::vector<FacePart> faceParts;
    const uint32_t blendColor(const uint32_t &first, const uint32_t &second);

public:
    /* Методы чтения/записи JSON */
    // Возвращает `true` если анимация успешно загружена и `false` в противном случае
    bool loadAnimationsFromJson(const std::string &filePath, std::vector<FacePart> &faceParts);
    // Возвращает `true` если анимация успешно сохранена и `false` в противном случае
    bool saveAnimationsToJson(const std::string &filePath, const std::vector<FacePart> &faceParts);

    // Метод наложения кадра на общий буфер
    void overlayFrameOntoBuffer(const std::vector<uint32_t> &frame, uint32_t *buffer,
                                const std::pair<uint16_t, uint16_t> &frameDimensions,
                                const std::pair<uint16_t, uint16_t> &bufferDimensions,
                                std::pair<uint16_t, uint16_t> &position);
    // Отрисовка всех частей тела
    void renderToBuffer(uint32_t *buffer, const std::pair<uint16_t, uint16_t> &bufferDimensions);

    // Замена части лица
    void replaceFacePart(size_t index, const FacePart &newPart);
};
