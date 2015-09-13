import java.util.Scanner;
class Nirvana extends Thread {
    public boolean enlightenment = false;
    public void run() {
        try {
            meditate();
        } catch (InterruptedException e) {
        }
    }
    synchronized void meditate() throws InterruptedException {
        while (!enlightenment) {
            System.out.println("Om...");
            wait();
        }
    }
    synchronized void enlighten() {
        enlightenment = true;
        notify();
    }
    public static void main(String[] args) throws InterruptedException {
        Nirvana t = new Nirvana();
        t.start();
        Scanner s = new Scanner(System.in);
        s.nextLine();
        t.enlighten();
        t.join();
    }
}