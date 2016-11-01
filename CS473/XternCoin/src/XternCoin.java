/**
 * Created by shreyaanshbassi on 10/30/16.
 */

import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;

public class XternCoin {

    private static int number;
    private ArrayList<XternUser> userList;
    private static ServerSocket serverSocket;
    private static Socket clientSocket = null;

    private void setNumber() {
        Random rand = new Random();
        number = rand.nextInt(10) + 1;
    }

    public Boolean HandleGuess(int guess) {
        if (guess == number) return true;
        return false;
    }

    public int GetCoins(XternUser user) {
        return user.numOfCoins;
    }

    public void UpdateCoin(XternUser user) {
        user.numOfCoins++;
        return;
    }

    public void StartGuessing(int uid) {
        XternUser user = new XternUser("Random User");
        userList.add(user);
        while (true) {
            for (int i = 1; i <= 10; i++) {
                if (HandleGuess(i)) {
                    UpdateCoin(user);
                }
            }
        }
    }

    public XternCoin() {
        userList = new ArrayList<XternUser>();
    }

    public static void main(String[] args)
    {
        try {
            serverSocket = new ServerSocket(1234);

        } catch (Exception e) {

        }

        while (true) {
            try {
                clientSocket = serverSocket.accept();

                Thread t = new Thread();

                t.start();
            } catch (Exception e) {
                System.out.println("Error in connection attempt.");
            }
        }
    }
}
