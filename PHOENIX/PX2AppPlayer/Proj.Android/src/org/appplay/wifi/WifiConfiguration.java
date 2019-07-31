package org.appplay.wifi;

import java.util.BitSet;

import android.net.wifi.WifiEnterpriseConfig;
import android.os.Parcel;
import android.os.Parcelable;


public class WifiConfiguration implements Parcelable {
    public String BSSID;
    public String FQDN;
    public String SSID;
    public BitSet allowedAuthAlgorithms;
    public BitSet allowedGroupCiphers;
    public BitSet allowedKeyManagement;
    public BitSet allowedPairwiseCiphers;
    public BitSet allowedProtocols;
    public WifiEnterpriseConfig enterpriseConfig;
    public boolean hiddenSSID;
    public int networkId;
    public String preSharedKey;
    public int priority;
    public int status;
    public String[] wepKeys = null;
    public int wepTxKeyIndex;

    public WifiConfiguration() {
        throw new RuntimeException("Stub!");
    }

    public String toString() {
        throw new RuntimeException("Stub!");
    }

    public int describeContents() {
        throw new RuntimeException("Stub!");
    }

    public void writeToParcel(Parcel dest, int flags) {
        throw new RuntimeException("Stub!");
    }

    public static class Status {
        public static final int CURRENT = 0;
        public static final int DISABLED = 1;
        public static final int ENABLED = 2;
        public static final String[] strings = null;

        Status() {
            throw new RuntimeException("Stub!");
        }
    }

    public static class GroupCipher {
        public static final int CCMP = 3;
        public static final int TKIP = 2;
        public static final int WEP104 = 1;
        public static final int WEP40 = 0;
        public static final String[] strings = null;
        public static final String varName = "group";

        GroupCipher() {
            throw new RuntimeException("Stub!");
        }
    }

    public static class PairwiseCipher {
        public static final int CCMP = 2;
        public static final int NONE = 0;
        public static final int TKIP = 1;
        public static final String[] strings = null;
        public static final String varName = "pairwise";

        PairwiseCipher() {
            throw new RuntimeException("Stub!");
        }
    }

    public static class AuthAlgorithm {
        public static final int LEAP = 2;
        public static final int OPEN = 0;
        public static final int SHARED = 1;
        public static final String[] strings = null;
        public static final String varName = "auth_alg";

        AuthAlgorithm() {
            throw new RuntimeException("Stub!");
        }
    }

    public static class Protocol {
        public static final int RSN = 1;
        public static final int WPA = 0;
        public static final String[] strings = null;
        public static final String varName = "proto";

        Protocol() {
            throw new RuntimeException("Stub!");
        }
    }

    public static class KeyMgmt {
        public static final int IEEE8021X = 3;
        public static final int NONE = 0;
        public static final int WPA_EAP = 2;
        public static final int WPA_PSK = 1;
        public static final String[] strings = null;
        public static final String varName = "key_mgmt";

        KeyMgmt() {
            throw new RuntimeException("Stub!");
        }
    }
}
