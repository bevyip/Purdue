import java.io.*;

public class Recursion {
    static int count = 0;
    
    public static int power(int a, int b) {
        return (b == 0) ? 1 : a * power(a, b - 1);
    }
    
    public static int fileCount(File f) {
        File[] file = f.listFiles();
        for (int i = 0; i < file.length; i++) {
            if (file[i].isDirectory()) {
                count++;
                fileCount(file[i]);
            } else {
                count++;
            }
        }
        return count;
    }
    
    public static int mysterySeries(int i, int j) {
        if (i < 0 || j < 0 || i < j) return 0;
        if (i == j || j == 0) return 1;
        else return mysterySeries(i - 1, j) + mysterySeries(i - 1, j - 1);
    }
    
    public static void hanoi(int n, char src, char dest, char aux) {
        if (n == 0) {
            return;
        } else {
            hanoi(n - 1, src, aux, dest);
            System.out.printf("Move disk %d from tower %s to tower %s%n", n, src, dest);
            hanoi(n - 1, aux, dest, src);
        }
    }
    
    public static void main(String[] args) {
        File f = new File("/homes/sbassi/cs180");
        System.out.println(power(2, 4));
        System.out.println(fileCount(f));
        System.out.println(mysterySeries(6, 3));
        hanoi(0, 'A', 'C', 'B');
    }
    
}