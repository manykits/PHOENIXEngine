/*****************************************************************************
 * LibVLC.java
 *****************************************************************************
 * Copyright © 2010-2013 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

package org.videolan.libvlc;

import java.util.ArrayList;

import android.os.Build;
import android.util.Log;
import android.view.Surface;

import org.videolan.libvlc.util.HWDecoderUtil;

public class LibVLC extends VLCObject<LibVLC.Event> {
    private static final String TAG = "VLC/LibVLC";

    public static class Event extends VLCEvent {
        protected Event(int type) {
            super(type);
        }
    }

    /** Native crash handler */
    private static OnNativeCrashListener sOnNativeCrashListener;

    public interface HardwareAccelerationError {
        void eventHardwareAccelerationError(); // TODO REMOVE
    }

    /**
     * Create a LibVLC withs options
     *
     * @param options
     */
    public LibVLC(ArrayList<String> options) {
        boolean setAout = true, setChroma = true;
        // check if aout/vout options are already set
        if (options != null) {
            for (String option : options) {
                if (option.startsWith("--aout="))
                    setAout = false;
                if (option.startsWith("--androidwindow-chroma"))
                    setChroma = false;
                if (!setAout && !setChroma)
                    break;
            }
        }

        // set aout/vout options if they are not set
        if (setAout || setChroma) {
            if (options == null)
                options = new ArrayList<String>();
            if (setAout) {
                final HWDecoderUtil.AudioOutput hwAout = HWDecoderUtil.getAudioOutputFromDevice();
                if (hwAout == HWDecoderUtil.AudioOutput.OPENSLES)
                    options.add("--aout=opensles");
                else
                    options.add("--aout=android_audiotrack");
            }
            if (setChroma) {
                options.add("--androidwindow-chroma");
                options.add("RV32");
            }
        }

        nativeNew(options.toArray(new String[options.size()]));
    }

    /**
     * Create a LibVLC
     */
    public LibVLC() {
        this(null);
    }

    public void setOnHardwareAccelerationError(HardwareAccelerationError error) {
        nativeSetOnHardwareAccelerationError(error);
    }
    private native void nativeSetOnHardwareAccelerationError(HardwareAccelerationError error);

    /**
     * Get the libVLC version
     * @return the libVLC version string
     */
    public native String version();

    /**
     * Get the libVLC compiler
     * @return the libVLC compiler string
     */
    public native String compiler();

    /**
     * Get the libVLC changeset
     * @return the libVLC changeset string
     */
    public native String changeset();

    @Override
    protected Event onEventNative(int eventType, long arg1, float arg2) {
        return null;
    }

    @Override
    protected void onReleaseNative() {
        nativeRelease();
    }

    public static interface OnNativeCrashListener {
        public void onNativeCrash();
    }

    public static void setOnNativeCrashListener(OnNativeCrashListener l) {
        sOnNativeCrashListener = l;
    }

    private static void onNativeCrash() {
        if (sOnNativeCrashListener != null)
            sOnNativeCrashListener.onNativeCrash();
    }

    /**
     * Sets the application name. LibVLC passes this as the user agent string
     * when a protocol requires it.
     *
     * @param name human-readable application name, e.g. "FooBar player 1.2.3"
     * @param http HTTP User Agent, e.g. "FooBar/1.2.3 Python/2.6.0"
     */
    public void setUserAgent(String name, String http){
        nativeSetUserAgent(name, http);
    }

    /* JNI */
    private native void nativeNew(String[] options);
    private native void nativeRelease();
    private native void nativeSetUserAgent(String name, String http);
}
