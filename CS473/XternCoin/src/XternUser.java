/**
 * Created by shreyaanshbassi on 10/30/16.
 */

import java.util.*;

public class XternUser {
    String uName;
    String password;
    int numOfCoins;
    ArrayList<Boolean> attempt;

    public XternUser (String name) {
        uName = name;
        numOfCoins = 0;
        attempt = new ArrayList<Boolean>();
    }
}
