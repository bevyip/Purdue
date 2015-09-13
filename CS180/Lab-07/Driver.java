public class Driver extends Thread {
    public static void main(String[] args) {
        Thread t1 = new A();
        t1.start();
        Runnable r = new B();
        Thread t = new Thread(r);
        t.start();
    }
}