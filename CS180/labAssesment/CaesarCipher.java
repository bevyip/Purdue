public class CaesarCipher {
    public static String encrypt (String plaintext, int key) {
        String ciphertext = "";
        char d;
        for (int i = 0; i < plaintext.length(); i++) {
            if ((int) plaintext.charAt(i) >= 90 - key) {
                d = (char) (plaintext.charAt(i) - 26 + key);
            } else {
                d = (char) (plaintext.charAt(i) + key);
            }
            ciphertext += d;
        }
        return ciphertext;
    }
    public static String decrypt (String plaintext, int key) {
        String ciphertext = "";
        char d;
        for (int i = 0; i < plaintext.length(); i++) {
            if ((int) plaintext.charAt(i) <= 64 + key) {
                d = (char) (plaintext.charAt(i) + 26 - key);
            } else {
                d = (char) (plaintext.charAt(i) - key);
            }
            ciphertext += d;
        }
        return ciphertext;
    }
    public static String justTheLetters (String text) {
        String plaintext = "";
        char d;
        for (int i = 0; i < text.length(); i++) {
            d = text.charAt(i);
            if (Character.isLetter(d))
                plaintext += d;
        }
        return plaintext.toUpperCase();
    }
    public static int crack (String ciphertext) {
        int[] keys = new int[26];
        int s = 0;
        int key = 0;
        char d;
        for (int i = 0; i < ciphertext.length(); i++) {
            d = ciphertext.charAt(i);
            if (d == 'A')
                keys[0]++;
            if (d == 'B')
                keys[1]++;
            if (d == 'C')
                keys[2]++;
            if (d == 'D')
                keys[3]++;
            if (d == 'E')
                keys[4]++;
            if (d == 'F')
                keys[5]++;
            if (d == 'G')
                keys[6]++;
            if (d == 'H')
                keys[7]++;
            if (d == 'I')
                keys[8]++;
            if (d == 'J')
                keys[9]++;
            if (d == 'K')
                keys[10]++;
            if (d == 'L')
                keys[11]++;
            if (d == 'M')
                keys[12]++;
            if (d == 'N')
                keys[13]++;
            if (d == 'O')
                keys[14]++;
            if (d == 'P')
                keys[15]++;
            if (d == 'Q')
                keys[16]++;
            if (d == 'R')
                keys[17]++;
            if (d == 'S')
                keys[18]++;
            if (d == 'T')
                keys[19]++;
            if (d == 'U')
                keys[20]++;
            if (d == 'V')
                keys[21]++;
            if (d == 'W')
                keys[22]++;
            if (d == 'X')
                keys[23]++;
            if (d == 'Y')
                keys[24]++;
            if (d == 'Z')
                keys[25]++;
        }
        for (int z = 0; z < keys.length; z++) {
            int highest = 99999;
            int x = keys[z];
            if ( highest < x ) {
                highest = x;
                int w = z;
            }
            s = (7 - z);
        }
        key = s + 26;
        return key;
    }
    public static void main(String[] args) {
        CaesarCipher c = new CaesarCipher();
        System.out.println(c.encrypt("ZEBRA", 3));
        System.out.println(c.decrypt("CHEUD", 3));
        System.out.println(c.encrypt(justTheLetters("2349aB% ^&cd"), 1));
        System.out.println(c.crack("BPWAMEPWKIUMJMNWZMCAUILMKMZBIQVBPIBBPQAKWCVBZGZWLMBPMNQZABEIDMA" +
                "WNBPMQVLCABZQITZMDWTCBQWVABPMNQZABEIDMAWNUWLMZVQVDMVBQWVIVLBPMN" +
                "QZABEIDMWNVCKTMIZXWEMZIVLBPQAOMVMZIBQWVLWMAVWBQVBMVLBWNWCVLMZQV" +
                "BPMJIKSEIAPWNBPMKWUQVOIOMWNAXIKMEMUMIVBWJMIXIZBWNQBEMUMIVBWTMIL" +
                "QBNWZBPMMGMAWNBPMEWZTLVWETWWSQVBWAXIKMBWBPMUWWVIVLBWBPMXTIVMBAJ" +
                "MGWVLIVLEMPIDMDWEMLBPIBEMAPITTVWBAMMQBOWDMZVMLJGIPWABQTMNTIOWNK" +
                "WVYCMABJCBJGIJIVVMZWNNZMMLWUIVLXMIKMEMPIDMDWEMLBPIBEMAPITTVWBAM" +
                "MAXIKMNQTTMLEQBPEMIXWVAWNUIAALMABZCKBQWVJCBEQBPQVABZCUMVBAWNSVW" +
                "ETMLOMIVLCVLMZABIVLQVOGMBBPMDWEAWNBPQAVIBQWVKIVWVTGJMNCTNQTTMLQ" +
                "NEMQVBPQAVIBQWVIZMNQZABIVLBPMZMNWZMEMQVBMVLBWJMNQZABQVAPWZBWCZT" +
                "MILMZAPQXQVAKQMVKMIVLQVQVLCABZGWCZPWXMANWZXMIKMIVLAMKCZQBGWCZWJ" +
                "TQOIBQWVABWWCZAMTDMAIAEMTTIAWBPMZAITTZMYCQZMCABWUISMBPQAMNNWZBB" +
                "WAWTDMBPMAMUGABMZQMABWAWTDMBPMUNWZBPMOWWLWNITTUMVIVLBWJMKWUMBPM" +
                "EWZTLATMILQVOAXIKMNIZQVOVIBQWVEMAMBAIQTWVBPQAVMEAMIJMKICAMBPMZM" +
                "QAVMESVWETMLOMBWJMOIQVMLIVLVMEZQOPBABWJMEWVIVLBPMGUCABJMEWVIVLC" +
                "AMLNWZBPMXZWOZMAAWNITTXMWXTMNWZAXIKMAKQMVKMTQSMVCKTMIZAKQMVKMIV" +
                "LITTBMKPVWTWOGPIAVWKWVAKQMVKMWNQBAWEVEPMBPMZQBEQTTJMKWUMINWZKMN" +
                "WZOWWLWZQTTLMXMVLAWVUIVIVLWVTGQNBPMCVQBMLABIBMAWKKCXQMAIXWAQBQW" +
                "VWNXZMMUQVMVKMKIVEMPMTXLMKQLMEPMBPMZBPQAVMEWKMIVEQTTJMIAMIWNXMI" +
                "KMWZIVMEBMZZQNGQVOBPMIBMZWNEIZQLWVWBAIGBPMEMAPWCTLWZEQTTOWCVXZW" +
                "BMKBMLIOIQVABBPMPWABQTMUQACAMWNAXIKMIVGUWZMBPIVEMOWCVXZWBMKBMLI" +
                "OIQVABBPMPWABQTMCAMWNTIVLWZAMIJCBQLWAIGBPIBAXIKMKIVJMMFXTWZMLIV" +
                "LUIABMZMLEQBPWCBNMMLQVOBPMNQZMAWNEIZEQBPWCBZMXMIBQVOBPMUQABISMA" +
                "BPIBUIVPIAUILMQVMFBMVLQVOPQAEZQBIZWCVLBPQAOTWJMWNWCZA"));
    }
}