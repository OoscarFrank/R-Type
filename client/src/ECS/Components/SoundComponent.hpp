#pragma once

#include "SFML/Audio.hpp"

#pragma once

#include <memory>
#include <string>
#include "SFML/Audio.hpp"

namespace ECS {
    namespace components {
        class SoundComponent {
            public:
                /**
                 * @brief Construct a new Sound Component object
                 *
                 */
                SoundComponent(const std::string &soundPath, float volume = 100.0f): _soundPath(soundPath) {
                    _soundBuffer = std::make_shared<sf::SoundBuffer>();
                    _soundBuffer->loadFromFile(_soundPath);

                    _sound = std::make_shared<sf::Sound>();
                    _sound->setBuffer(*_soundBuffer);
                    _sound->setVolume(volume);
                }

                int playSound() {
                    _sound->play();
                    return 0;
                }

                int pauseSound() {
                    if (_sound->getStatus() != sf::Sound::Playing)
                        return 84;
                    _sound->pause();
                    return 0;
                }

                int stopSound() {
                    if (_sound->getStatus() != sf::Sound::Playing)
                        return 84;
                    _sound->stop();
                    return 0;
                }

                void setVolume(float volume) {
                    _sound->setVolume(volume);
                }

                sf::SoundSource::Status getStatus() const {
                    return _sound->getStatus();
                }

                float getVolume() const {
                    return _sound->getVolume();
                }

            private:
                std::string _soundPath;
                std::shared_ptr<sf::SoundBuffer> _soundBuffer;
                std::shared_ptr<sf::Sound> _sound;
        };
    }
}
