package com.kevin.androidffmpegplayer.decoder;

import java.nio.ByteBuffer;

public class FrameInfo {
    int buffer;
    ByteBuffer data;
    long presentationTimeUs;
    boolean endOfStream;
    boolean representationChanged;

    public int getBuffer() {
        return buffer;
    }

    public ByteBuffer getData() {
        return data;
    }

    public long getPresentationTimeUs() {
        return presentationTimeUs;
    }

    public boolean isEndOfStream() {
        return endOfStream;
    }

    public boolean isRepresentationChanged() {
        return representationChanged;
    }

    public FrameInfo() {
        clear();
    }

    public void clear() {
        buffer = -1;
        data = null;
        presentationTimeUs = -1;
        endOfStream = false;
        representationChanged = false;
    }
}
