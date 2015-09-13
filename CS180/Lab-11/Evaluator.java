import java.util.*;

public class Evaluator {
    public static int evaluate(String s) {
        String[] string = s.split(" ");
        Stack<Integer> st = new Stack<Integer>();
        for (int i = 0; i < string.length; i++) {
            try {
                int temp = Integer.parseInt(string[i]);
                st.push(temp);
            } catch (NumberFormatException e) {
                int tempo = 0;
                switch (string[i]) {
                    case "+": {tempo = st.pop() + st.pop(); st.push(tempo); break;}
                    case "-": {tempo = st.pop() - st.pop(); st.push(-tempo); break;}
                    case "/": {tempo = st.pop() / st.pop(); st.push(tempo); break;}
                    case "*": {tempo = st.pop() * st.pop(); st.push(tempo); break;}
                    case " ": break;
                    default: {System.out.println("Error");} 
                }  
            }
        }
        return st.pop();
    }
    public static void main(String[] args) {
        System.out.printf("%d",evaluate("51 1 2 + 4 * + 3 -"));
    }
}