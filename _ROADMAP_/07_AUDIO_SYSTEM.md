# Phase 7: Audio System - Detailed Design Document

## Overview
A robust audio system using OpenAL for handling sound effects, music, and 3D positional audio.

## 1. Core Audio System

### Audio Engine
```cpp
class AudioEngine {
private:
    ALCdevice* m_Device;
    ALCcontext* m_Context;
    std::vector<AudioSource*> m_Sources;
    std::unordered_map<std::string, AudioBuffer*> m_Buffers;
    
public:
    bool Init();
    void Shutdown();
    void Update();
    
    AudioSource* CreateSource();
    AudioBuffer* LoadBuffer(const std::string& path);
    void SetListenerPosition(const Vector3D& position);
    void SetListenerOrientation(const Vector3D& forward, const Vector3D& up);
};
```

### Audio Buffer
```cpp
class AudioBuffer {
private:
    ALuint m_BufferID;
    AudioFormat m_Format;
    int m_SampleRate;
    std::vector<uint8_t> m_Data;
    
public:
    bool LoadFromFile(const std::string& path);
    bool LoadFromMemory(const void* data, size_t size);
    void Unload();
    
    ALuint GetBufferID() const { return m_BufferID; }
    AudioFormat GetFormat() const { return m_Format; }
    int GetSampleRate() const { return m_SampleRate; }
};
```

### Audio Source
```cpp
class AudioSource {
private:
    ALuint m_SourceID;
    AudioBuffer* m_Buffer;
    bool m_Looping;
    float m_Volume;
    float m_Pitch;
    Vector3D m_Position;
    Vector3D m_Velocity;
    
public:
    void SetBuffer(AudioBuffer* buffer);
    void Play();
    void Stop();
    void Pause();
    void Resume();
    
    void SetPosition(const Vector3D& position);
    void SetVelocity(const Vector3D& velocity);
    void SetVolume(float volume);
    void SetPitch(float pitch);
    void SetLooping(bool loop);
    
    bool IsPlaying() const;
    bool IsPaused() const;
};
```

## 2. Sound Management

### Sound Manager
```cpp
class SoundManager {
private:
    struct SoundData {
        std::string Path;
        float BaseVolume;
        float MinDistance;
        float MaxDistance;
        bool Positional;
    };
    
    std::unordered_map<std::string, SoundData> m_SoundDatabase;
    ObjectPool<AudioSource> m_SourcePool;
    
public:
    void LoadSoundDatabase(const std::string& path);
    AudioSource* PlaySound(const std::string& name);
    AudioSource* PlaySoundAtPosition(
        const std::string& name,
        const Vector3D& position
    );
    
    void StopAllSounds();
    void PauseAllSounds();
    void ResumeAllSounds();
};
```

### Music Manager
```cpp
class MusicManager {
private:
    struct MusicTrack {
        std::string Path;
        float FadeInTime;
        float FadeOutTime;
        float Volume;
        bool Loop;
    };
    
    AudioSource* m_CurrentMusic;
    AudioSource* m_NextMusic;
    std::unordered_map<std::string, MusicTrack> m_Tracks;
    float m_TransitionTime;
    float m_CurrentFade;
    
public:
    void LoadMusicDatabase(const std::string& path);
    void PlayMusic(const std::string& name, bool immediate = false);
    void StopMusic(bool immediate = false);
    void PauseMusic();
    void ResumeMusic();
    
    void SetMusicVolume(float volume);
    void UpdateTransition(float deltaTime);
};
```

## 3. Audio Components

### Audio Listener Component
```cpp
class AudioListenerComponent : public Component {
private:
    Vector3D m_Forward;
    Vector3D m_Up;
    
public:
    void Update(float deltaTime) override;
    void SetOrientation(const Vector3D& forward, const Vector3D& up);
};
```

### Audio Source Component
```cpp
class AudioSourceComponent : public Component {
private:
    AudioSource* m_Source;
    std::string m_CurrentSound;
    bool m_AutoPlay;
    bool m_PlayOnAwake;
    float m_MinDistance;
    float m_MaxDistance;
    
public:
    void PlaySound(const std::string& name);
    void StopSound();
    void SetAutoPlay(bool autoPlay);
    void Update(float deltaTime) override;
};
```

## 4. Sound Effects System

### Sound Effect Types
```cpp
namespace SoundEffects {
    class FootstepSystem {
    private:
        struct SurfaceData {
            std::vector<std::string> Sounds;
            float Volume;
            float PitchVariation;
        };
        
        std::unordered_map<std::string, SurfaceData> m_Surfaces;
        
    public:
        void LoadSurfaceData(const std::string& path);
        void PlayFootstep(const std::string& surface, const Vector3D& position);
    };
    
    class ImpactSystem {
    private:
        struct ImpactData {
            std::vector<std::string> Sounds;
            float VolumeScale;
            float PitchScale;
        };
        
    public:
        void PlayImpact(
            const std::string& material1,
            const std::string& material2,
            float intensity,
            const Vector3D& position
        );
    };
}
```

## 5. Audio Mixing

### Audio Mixer
```cpp
class AudioMixer {
public:
    enum class Channel {
        Master,
        Music,
        SFX,
        Ambient,
        Voice
    };
    
private:
    struct ChannelData {
        float Volume;
        float VolumeModifier;
        bool Muted;
        std::vector<AudioSource*> Sources;
    };
    
    std::unordered_map<Channel, ChannelData> m_Channels;
    
public:
    void SetChannelVolume(Channel channel, float volume);
    void SetChannelMute(Channel channel, bool mute);
    void AddSourceToChannel(AudioSource* source, Channel channel);
    void RemoveSourceFromChannel(AudioSource* source, Channel channel);
    
    void Update();
};
```

## Implementation Timeline

### Week 1
- Day 1-2: Core audio engine setup
- Day 3-4: Sound and music managers
- Day 5-7: Basic audio playback system

### Week 2
- Day 1-2: 3D positional audio
- Day 3-4: Audio components
- Day 5-7: Sound effects system and mixing

## Performance Targets

### Audio Performance
- Maximum concurrent sources: 32
- Latency < 50ms
- CPU usage < 5%
- Memory usage < 100MB
- Buffer loading time < 100ms

### Quality Targets
- Sample rate: 44.1kHz
- Bit depth: 16-bit
- Stereo output
- Distance attenuation
- Doppler effect support

## Testing Strategy

### Unit Tests
- Audio file loading
- Source management
- Buffer allocation
- Volume control
- Position calculations

### Integration Tests
- 3D audio positioning
- Music transitions
- Multiple sound playback
- Component integration
- Resource management

### Performance Tests
- Memory usage
- CPU utilization
- Loading times
- Concurrent source limit
- Audio latency

## Debug Features

### Audio Debugger
```cpp
class AudioDebugger {
public:
    void ShowAudioSources();
    void ShowBufferStatus();
    void ShowChannelVolumes();
    void ShowPositionalAudio();
    void ShowMemoryUsage();
};
```

### Visualization
- Audio source positions
- Listener orientation
- Attenuation ranges
- Active sources
- Channel levels

## Audio Data Format (JSON)
```json
{
    "sounds": {
        "player_jump": {
            "file": "sounds/jump.ogg",
            "volume": 0.8,
            "pitch": 1.0,
            "min_distance": 1.0,
            "max_distance": 20.0,
            "positional": true,
            "channel": "SFX"
        },
        "background_music": {
            "file": "music/level1.ogg",
            "volume": 0.5,
            "fade_in": 2.0,
            "fade_out": 1.5,
            "loop": true,
            "channel": "Music"
        }
    },
    "surfaces": {
        "grass": {
            "footsteps": [
                "sounds/footstep_grass_01.ogg",
                "sounds/footstep_grass_02.ogg",
                "sounds/footstep_grass_03.ogg"
            ],
            "volume": 0.7,
            "pitch_variation": 0.1
        }
    },
    "mixer": {
        "master": {
            "volume": 1.0,
            "muted": false
        },
        "music": {
            "volume": 0.8,
            "muted": false
        },
        "sfx": {
            "volume": 1.0,
            "muted": false
        }
    }
}
```
