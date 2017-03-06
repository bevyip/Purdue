/**
 * Created by shreyaansh on 2/12/17.
 */
import java.util.*;

public class Lexer {

    private String input;
    private String[] splitTokens;
    private ArrayList<Tokens> tokens;
    private int i;

    public Lexer(String input) {
        i = 0;
        this.input = input;

        //Removing spaces from the input
        this.input = this.input.replace(" ", "");
        if (this.input.length() == 0) throw new Error("No input!");

        splitTokens = this.input.split("");

        tokens = new ArrayList<Tokens>();

        automaton();
    }

    public ArrayList<Tokens> getTokens() { return tokens; }

    private enum States { START, NUM, NUM_DOUBLE, DEC, ID, ID_NUM };

    private void advance() { i++; }

    private void consumeToken(String token, String tokenType) { tokens.add(new Tokens(token, tokenType)); }

    private void automaton() {
        States state;
        state = States.START;

        StringBuilder sb = new StringBuilder();
        char c;

        boolean endOfString = false;

        while (i <= splitTokens.length) {
            if (i < splitTokens.length) {
                c = splitTokens[i].charAt(0);
            } else {
                c = '\0';
                endOfString = true;
            }
            switch (state) {
                case START:
                    if (Character.isDigit(c)) {
                        sb.append(c);
                        state = States.NUM;
                        advance();
                    } else if (Character.isAlphabetic(c)) {
                        sb.append(c);
                        state = States.ID;
                        advance();
                    } else {
                        switch (c) {
                            case '+':
                                consumeToken("+", "ADD");
                                break;
                            case '-':
                                consumeToken("-", "SUB");
                                break;
                            case '/':
                                consumeToken("/", "DIV");
                                break;
                            case '*':
                                consumeToken("*", "MUL");
                                break;
                            case '^':
                                consumeToken("^", "POW");
                                break;
                            case ',':
                                consumeToken(",", "COMMA");
                                break;
                            case '(':
                                consumeToken("(", "LPAREN");
                                break;
                            case ')':
                                consumeToken(")", "RPAREN");
                                break;
                            case '\0':
                                advance();
                                break;
                            default:
                                throw new Error("Incorrect Input: Token not defined");
                        }
                        state = States.START;
                        advance();
                    }
                    break;
                case NUM:
                    if (Character.isDigit(c)) {
                        sb.append(c);
                        state = States.NUM;
                        advance();
                    } else if (c == '.') {
                        sb.append(c);
                        state = States.DEC;
                        advance();
                    } else {
                        state = States.START;
                        consumeToken(sb.toString(), "NUM");
                        sb.setLength(0);
                        if (endOfString) advance();
                    }
                    break;
                case DEC:
                    if (Character.isDigit(c)) {
                        sb.append(c);
                        state = States.NUM_DOUBLE;
                        advance();
                    } else {
                        throw new Error("Incorrect Input: Token not defined");
                    }
                    break;
                case NUM_DOUBLE:
                    if (Character.isDigit(c)) {
                        sb.append(c);
                        state = States.NUM_DOUBLE;
                        advance();
                    } else {
                        state = States.START;
                        consumeToken(sb.toString(), "NUM_DOUBLE");
                        sb.setLength(0);
                        if (endOfString) advance();
                    }
                    break;
                case ID:
                    if (Character.isAlphabetic(c)) {
                        sb.append(c);
                        state = States.ID;
                        advance();
                    } else if (Character.isDigit(c)) {
                        sb.append(c);
                        state = States.ID_NUM;
                        advance();
                    } else {
                        state = States.START;
                        String ID_TYPE = sb.toString();
                        switch (ID_TYPE) {
                            case "sin":
                                consumeToken(ID_TYPE, "SIN");
                                if (endOfString) advance();
                                break;
                            case "cos":
                                consumeToken(ID_TYPE, "COS");
                                if (endOfString) advance();
                                break;
                            case "tan":
                                consumeToken(ID_TYPE, "TAN");
                                if (endOfString) advance();
                                break;
                            case "ln":
                                consumeToken(ID_TYPE, "LN");
                                if (endOfString) advance();
                                break;
                            case "atan":
                                consumeToken(ID_TYPE, "ATAN");
                                if (endOfString) advance();
                                break;
                            default:
                                consumeToken(ID_TYPE, "ID");
                                sb.setLength(0);
                                if (endOfString) advance();
                                break;
                        }
                        sb.setLength(0);
                    }
                    break;
                case ID_NUM:
                    if (Character.isDigit(c)) {
                        sb.append(c);
                        state = States.ID_NUM;
                        advance();
                    } else {
                        state = States.START;
                        String ID_TYPE = sb.toString();
                        if (ID_TYPE.equals("atan2")) consumeToken(ID_TYPE, "ATAN2");
                        else consumeToken(ID_TYPE, "ID");
                        sb.setLength(0);
                        if (endOfString) advance();
                    }
                    break;
                default:
                    throw new Error("Incorrect Input: Token not defined");
            }
        }
    }

    public static void main(String[] args) {
        Lexer lex = new Lexer("x+(y-sin(1232))");
        lex.automaton();
        for (Tokens tok : lex.tokens) {
            System.out.println("Token ID: " + tok.TokenType);
            System.out.println("Token Type: " + tok.Token);
        }
    }
}