package com.kevin.androidffmpegplayer.frame;

import java.nio.ByteBuffer;

/**
 * Created by XieJiaHua on 19-4-29
 */
public class FrameEntity {
    public static final int MEDIA_CODEC_VIDEO_H264 = 78;
    public static final int MEDIA_CODEC_VIDEO_H265 = 80;
    public static final int MEDIA_CODEC_AUDIO_AAC = 136;

    private P2pFrameHeader frameHeader;
    private byte[] frame;

    public FrameEntity(P2pFrameHeader frameHeader, byte[] frame) {
        this.frameHeader = frameHeader;
        this.frame = frame;
    }

    public FrameEntity() {

    }

    public P2pFrameHeader getFrameHeader() {
        return frameHeader;
    }

    public void setFrameHeader(P2pFrameHeader frameHeader) {
        this.frameHeader = frameHeader;
    }

    public byte[] getFrame() {
        return frame;
    }

    public void setFrame(byte[] frame) {
        this.frame = frame;
    }

    public boolean isValidFrame() {
        return frameHeader != null;
    }

    public boolean isVideoFrame() {
        return frameHeader.getCodec_id() == MEDIA_CODEC_VIDEO_H264 || frameHeader.getCodec_id() == MEDIA_CODEC_VIDEO_H265;
    }


    public boolean isAudioFrame() {
        return frameHeader.getCodec_id() == MEDIA_CODEC_AUDIO_AAC;
    }

    public byte[] toBytesArray() {
        byte[] header = frameHeader.toByteArray();
        byte[] allByteArray = new byte[header.length + frame.length];
        ByteBuffer buff = ByteBuffer.wrap(allByteArray);
        buff.put(header);
        buff.put(frame);
        return buff.array();
    }

    @Override
    public String toString() {
        return "FrameEntity{" +
                "type=" + (isVideoFrame() ? "video" : "audio") +
                ", frameHeader=" + frameHeader +
                '}';
    }
}
