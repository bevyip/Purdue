public class Test {
    public static int arraysize = 12;
    public static void main(String[] args) {
        
        int[] array = { 5, 7, 99, 12, 42, 31, 1, 45, 9, 21, 22, 23 };
        int i, j, temp;
        
        for (i = arraysize - 2; i > 0; i--) {
            for (j = i; j < arraysize - 1; j++) {
                if (array[j] > array[j + 1]) {
                    temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                } else {
                    break;
                }
            }
        }
        for (int z = 0; z < array.length; z++) {
            System.out.print(array[z] + ", ");
        }
    }
}