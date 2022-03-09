package com.kevin.androidffmpegplayer.frame;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by XieJiaHua on 19-4-28
 */
public class FrameHeader {
    short codec_id;      // 0，1 帧类型。136 aac, 80 h265, 78 h264
    byte flags;        // 2   0x01 I Frame, 0x00 other
    byte cam_index;      // 3
    byte onlineNum;      // 4 当前连接到该摄像头的数量
    byte iFrameIndex;      // 5   实时: 范围[51 , 100] 回放: 范围[151, 200]
    short backwardIndex;       // 6，7
    /*8*/
    int unuse_int;          // 8,9,10,11 时间戳毫秒部分
    int timestamp;       // 12,13,14,15 时间戳秒部分
    int videowidth;        // 16,17,18,19 视频帧宽
    int videoheight;       // 20,21,22,23 视频帧高
    /*16*/
    short frmNo;       //24, 25 帧的编号 范围[0,]
    //26,27
    int len;           // 28, 29, 30, 31 body length
    /*6*/

    public static final int LENGTH = 32;

    @Override
    public String toString() {
        return "P2pFrameHeader{" +
                "frmNo=" + frmNo +
                ", codec_id=" + codec_id +
                ", flags=" + flags +
                ", cam_index=" + cam_index +
                ", iFrameIndex=" + iFrameIndex +
                ", timestamp=" + getPresentationTimeMs() +
                '}';
    }

    public long getPresentationTimeMs() {
        return timestamp * 1000L + unuse_int;
    }

    public long getPresentationTimeUs() {
        return getPresentationTimeMs() * 1000;
    }

    public static FrameHeader parseToP2pFrameHeader(byte[] header) {
        FrameHeader frameHeader = new FrameHeader();
        return parseToP2pFrameHeader(header, frameHeader);
    }

    public static FrameHeader parseToP2pFrameHeader(byte[] header, FrameHeader frameHeader) {
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(header);
        try {
            ByteBuffer wrapped;

            byte[] byteEach = new byte[2];
            byteArrayInputStream.read(byteEach);//codecid
            wrapped = ByteBuffer.wrap(byteEach);
            frameHeader.setCodec_id(wrapped.order(ByteOrder.LITTLE_ENDIAN).getShort());

            byteEach = new byte[1];
            byteArrayInputStream.read(byteEach);//flags
            frameHeader.setFlags(byteEach[0]);

            byteArrayInputStream.read(byteEach);//cam_index
            frameHeader.setCam_index(byteEach[0]);

            byteArrayInputStream.read(byteEach);//onlineNum
            frameHeader.setOnlineNum(byteEach[0]);

            byteArrayInputStream.read(byteEach);//iFrameIndex
            frameHeader.setiFrameIndex(byteEach[0]);

            byteEach = new byte[2];
            byteArrayInputStream.read(byteEach);//backwardIndex
            wrapped = ByteBuffer.wrap(byteEach);
            frameHeader.setBackwardIndex(wrapped.order(ByteOrder.LITTLE_ENDIAN).getShort());


            byteEach = new byte[4];
            byteArrayInputStream.read(byteEach);//unuse_int
            wrapped = ByteBuffer.wrap(byteEach);
            frameHeader.setUnuse_int(wrapped.order(ByteOrder.LITTLE_ENDIAN).getInt());

            byteArrayInputStream.read(byteEach);//timestamp
            wrapped = ByteBuffer.wrap(byteEach);
            frameHeader.setTimestamp(wrapped.order(ByteOrder.LITTLE_ENDIAN).getInt());

            byteArrayInputStream.read(byteEach);//videowidth
            wrapped = ByteBuffer.wrap(byteEach);
            frameHeader.setVideowidth(wrapped.order(ByteOrder.LITTLE_ENDIAN).getInt());

            byteArrayInputStream.read(byteEach);//videoheight
            wrapped = ByteBuffer.wrap(byteEach);
            frameHeader.setVideoheight(wrapped.order(ByteOrder.LITTLE_ENDIAN).getInt());

            byteEach = new byte[2];
            byteArrayInputStream.read(byteEach);//frmNo
            wrapped = ByteBuffer.wrap(byteEach);
            frameHeader.setFrmNo(wrapped.order(ByteOrder.LITTLE_ENDIAN).getShort());


            byteEach = new byte[2];
            byteArrayInputStream.read(byteEach);//消除字节对齐

            byteEach = new byte[4];
            byteArrayInputStream.read(byteEach);//len
            wrapped = ByteBuffer.wrap(byteEach);
            frameHeader.setLen(wrapped.order(ByteOrder.LITTLE_ENDIAN).getInt());
        } catch (IOException e) {
            e.printStackTrace();
        }
        return frameHeader;
    }


    public byte[] toByteArray() {

        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        byte[] bytes = null;
        try {
            //以小端顺序写入
            byteArrayOutputStream.write(ByteBuffer.allocate(2).order(ByteOrder.LITTLE_ENDIAN).putShort(codec_id).array());
            byteArrayOutputStream.write(flags);
            byteArrayOutputStream.write(cam_index);
            byteArrayOutputStream.write(onlineNum);
            byteArrayOutputStream.write(iFrameIndex);
            byteArrayOutputStream.write(ByteBuffer.allocate(2).order(ByteOrder.LITTLE_ENDIAN).putShort(backwardIndex).array());
            byteArrayOutputStream.write(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(unuse_int).array());
            byteArrayOutputStream.write(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(timestamp).array());
            byteArrayOutputStream.write(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(videowidth).array());
            byteArrayOutputStream.write(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(videoheight).array());
            byteArrayOutputStream.write(ByteBuffer.allocate(2).order(ByteOrder.LITTLE_ENDIAN).putShort(frmNo).array());
            byteArrayOutputStream.write(ByteBuffer.allocate(2).order(ByteOrder.LITTLE_ENDIAN).putShort((short) 0).array());
            byteArrayOutputStream.write(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(len).array());
            bytes = byteArrayOutputStream.toByteArray();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return bytes;
    }

    public boolean isIFrame() {
        return flags == 1;
    }

    public short getCodec_id() {
        return codec_id;
    }

    public void setCodec_id(short codec_id) {
        this.codec_id = codec_id;
    }

    public byte getFlags() {
        return flags;
    }

    public void setFlags(byte flags) {
        this.flags = flags;
    }

    public byte getCam_index() {
        return cam_index;
    }

    public void setCam_index(byte cam_index) {
        this.cam_index = cam_index;
    }

    public byte getOnlineNum() {
        return onlineNum;
    }

    public void setOnlineNum(byte onlineNum) {
        this.onlineNum = onlineNum;
    }

    public byte getiFrameIndex() {
        return iFrameIndex;
    }

    public void setiFrameIndex(byte iFrameIndex) {
        this.iFrameIndex = iFrameIndex;
    }

    public short getBackwardIndex() {
        return backwardIndex;
    }

    public void setBackwardIndex(short backwardIndex) {
        this.backwardIndex = backwardIndex;
    }

    public int getUnuse_int() {
        return unuse_int;
    }

    public void setUnuse_int(int unuse_int) {
        this.unuse_int = unuse_int;
    }

    public int getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(int timestamp) {
        this.timestamp = timestamp;
    }

    public int getVideowidth() {
        return videowidth;
    }

    public void setVideowidth(int videowidth) {
        this.videowidth = videowidth;
    }

    public int getVideoheight() {
        return videoheight;
    }

    public void setVideoheight(int videoheight) {
        this.videoheight = videoheight;
    }

    public short getFrmNo() {
        return frmNo;
    }

    public void setFrmNo(short frmNo) {
        this.frmNo = frmNo;
    }

    public int getLen() {
        return len;
    }

    public void setLen(int len) {
        this.len = len;
    }

    public static int getLENGTH() {
        return LENGTH;
    }
}
