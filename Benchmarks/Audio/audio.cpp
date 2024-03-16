#include <iostream>
#include <AudioToolbox/AudioToolbox.h>

void playWavFile(const char* filename) {
    CFURLRef fileURL = CFURLCreateFromFileSystemRepresentation(NULL, (const UInt8*)filename, strlen(filename), false);
    if (!fileURL) {
        std::cerr << "Error: Could not create file URL.\n";
        return;
    }

    AudioFileID audioFile;
    OSStatus status = AudioFileOpenURL(fileURL, kAudioFileReadPermission, 0, &audioFile);
    CFRelease(fileURL);
    if (status != noErr) {
        std::cerr << "Error: Could not open audio file.\n";
        return;
    }

    AudioStreamBasicDescription format;
    UInt32 size = sizeof(format);
    status = AudioFileGetProperty(audioFile, kAudioFilePropertyDataFormat, &size, &format);
    if (status != noErr) {
        std::cerr << "Error: Could not get audio file format.\n";
        AudioFileClose(audioFile);
        return;
    }

    AudioQueueRef queue;
    status = AudioQueueNewOutput(&format, [](void*, AudioQueueRef, AudioQueueBufferRef){}, nullptr, nullptr, nullptr, 0, &queue);
    if (status != noErr) {
        std::cerr << "Error: Could not create audio queue.\n";
        AudioFileClose(audioFile);
        return;
    }

    const int bufferSize = 32768;
    AudioQueueBufferRef buffers[3];
    for (int i = 0; i < 3; ++i) {
        status = AudioQueueAllocateBuffer(queue, bufferSize, &buffers[i]);
        if (status != noErr) {
            std::cerr << "Error: Could not allocate audio queue buffer.\n";
            AudioQueueDispose(queue, true);
            AudioFileClose(audioFile);
            return;
        }

        // Read audio data into buffer
        UInt32 bytesRead = bufferSize;
        AudioFileReadBytes(audioFile, false, i * bufferSize, &bytesRead, buffers[i]->mAudioData);
        buffers[i]->mAudioDataByteSize = bytesRead;

        // Enqueue buffer
        status = AudioQueueEnqueueBuffer(queue, buffers[i], 0, nullptr);
        if (status != noErr) {
            std::cerr << "Error: Could not enqueue audio queue buffer.\n";
            AudioQueueDispose(queue, true);
            AudioFileClose(audioFile);
            return;
        }
    }

    // Start audio queue
    status = AudioQueueStart(queue, nullptr);
    if (status != noErr) {
        std::cerr << "Error: Could not start audio queue.\n";
        AudioQueueDispose(queue, true);
        AudioFileClose(audioFile);
        return;
    }

    // Wait for playback to finish
    AudioQueueFlush(queue);
    while (true) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, false);
        UInt32 isRunning;
        AudioQueueGetProperty(queue, kAudioQueueProperty_IsRunning, &isRunning, &size);
        if (!isRunning)
            break;
    }

    // Cleanup
    AudioQueueStop(queue, true);
    AudioQueueDispose(queue, true);
    AudioFileClose(audioFile);
}

int main() {
    const char* filename = "your_audio_file.wav"; // Replace with your WAV file path
    playWavFile(filename);
    return 0;
}
