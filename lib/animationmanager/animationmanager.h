#pragma once

#include <vector>
#include <string>

struct Frame
{
    std::vector<uint32_t> pixels;
};
struct FacePart
{
    std::string name;
    std::vector<Frame> frames; // Кадры анимации в порядке воспроизведения÷
    std::pair<uint16_t, uint16_t> position;
    std::pair<uint16_t, uint16_t> resolution; // Разрешение анимации
};
class AnimationManager
{
private:
    std::vector<FacePart> faceParts;

public:
    /*Методы чтения/записи JSON*/
    // Возвращает `true` если анимация успешно загружена и `false` в противном случае
    bool loadAnimationsFromJson(const std::string &filePath, std::vector<FacePart> &faceParts);
    // Возвращает `true` если анимация успешно сохранена и `false` в противном случае
    bool saveAnimationsToJson(const std::string &filePath, const std::vector<FacePart> &faceParts);

    // Метод наложения кадра на общий буфер
    void overlayFrameOntoBuffer(const Frame &frame, uint8_t *buffer,
                                const std::pair<uint16_t, uint16_t> &frameDimensions,
                                const std::pair<uint16_t, uint16_t> &bufferDimensions,
                                std::pair<uint16_t, uint16_t> &position);
};