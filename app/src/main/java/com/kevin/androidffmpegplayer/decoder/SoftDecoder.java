package com.kevin.androidffmpegplayer.decoder;

import com.kevin.androidffmpegplayer.jni.FFmpegDecoder;

/**
 * 实现软解，解码器
 */
public class SoftDecoder implements Decoder {
    boolean isStart = false;

    FFmpegDecoder fFmpegDecoder = new FFmpegDecoder();

//    public SoftDecoder() {
//
//    }

    @Override
    public void start() {
        if (!isStart) {
            reInitCodec();
//            fFmpegDecoder.start();

//            MediaFormat currentMediaFormat = mExtractor.getCurrentMediaFormat();
//            if (currentMediaFormat != null) {
//                mFormat = currentMediaFormat;
//            }
//            configureMediaCodec(mFormat);
//            mCodec.start();
//            mCodecInputBuffers = mCodec.getInputBuffers();
//            mCodecOutputBuffers = mCodec.getOutputBuffers();
            isStart = true;
        }
    }

    @Override
    public boolean isStart() {
        return isStart;
    }

    @Override
    public boolean reInitCodec() {
        /**
         * 根据h265或者h264切换解码器
         */
//        if (fFmpegDecoder != null) {
//            fFmpegDecoder.stop();
//        }
//        fF
        return false;
    }

    @Override
    public boolean feedInputBuffer() {
        if (!isStart) {
            return false;
        }

        //fFmpegDecoder.writePacket()
        return true;
    }

    @Override
    public FrameInfo drainOutputBuffer() {
        //FrameInfo info = fFmpegDecoder.readFrame();
        return null;
    }

    @Override
    public void renderFrame(FrameInfo frameInfo, long offsetUs) {

    }

    public void releaseFrame(FrameInfo frameInfo, boolean render) {
//        mCodec.releaseOutputBuffer(frameInfo.buffer, render); // render picture
//        releaseFrameInfo(frameInfo);

//        fFmpegDecoder.releaseFrame(frameInfo.buffer);
    }

    @Override
    public void stop() {
        if (isStart) {
//            try {
//                if (mCodec != null) {
//                    mCodec.stop();
//                    mCodec.reset();
//                }
//            } catch (IllegalStateException exception) {
//                Log.e(TAG, "", exception);
//            } finally {
//                mCodecInputBuffers = null;
//                mCodecOutputBuffers = null;
//                mOutputFormat = null;
//            }
//
//            mAvailableInputBufferIndices.clear();
//            mAvailableOutputBufferIndices.clear();
//            mAvailableOutputFrameInfos.clear();

//            fFmpegDecoder.stop();
            isStart = false;
        }
    }

    @Override
    public void flush() {
        if (!isStart()) {
            return;
        }
        //清空输入输出缓冲区
//        mAvailableInputBufferIndices.clear();
//        mAvailableOutputBufferIndices.clear();
//        mAvailableOutputFrameInfos.clear();
//
//        mSawInputEOS = false;
//        mSawOutputEOS = false;
//        try {
//            mCodec.flush();
//        }catch (IllegalStateException exception){
//            Log.e(TAG, "", exception);
//        }
    }

    @Override
    public void release() {
        stop();
//        if (mCodec != null) {
//            mCodec.reset();
//            mCodec.release();
//            Log.d(TAG, "release Codec. " + mCodec);
//            mCodec = null;
//        }
    }
}
