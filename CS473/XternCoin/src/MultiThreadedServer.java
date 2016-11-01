import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by shreyaanshbassi on 10/31/16.
 */

public class MultiThreadedServer implements Runnable {
    private Socket socket = null;

    public MultiThreadedServer (Socket socket) {
        this.socket = socket;
    }

    public static void main(String[] args) throws Exception {
        ServerSocket sock = new ServerSocket(1234);
        while (true) {
            Socket soc = sock.accept();
            new Thread(new MultiThreadedServer(soc)).start();
        }
    }

    public void run() {
        while (true) {
        }
    }
}
