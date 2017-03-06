
import java.awt.geom.Arc2D;
import java.lang.*;
import java.util.ArrayList;
import java.util.Iterator;

class ExprEval {

    public static ArrayList<Tokens> tokens;
    private static Iterator<Tokens> iter;
    private Tokens token;
    private Tokens nextToken;
    private double xval;

    private ExprEval(String expression, double xval) {
        Lexer lexer = new Lexer(expression);

        tokens = lexer.getTokens();
        iter = tokens.iterator();

        nextToken = iter.hasNext() ? iter.next() : new Tokens("END", "END");

        this.xval = xval;
    }

    public static void main(String args[]) throws Exception {
        if (args.length < 2) {
            System.out.println("Usage: java expression xval");
            System.exit(1);
        }

        ExprEval eval = new ExprEval(args[0], Double.parseDouble(args[1]));

        double val = eval.evaluate();
        System.out.println("y=" + val);
    }

    private void next() { nextToken = iter.hasNext() ? iter.next() : new Tokens("END", "END"); }

    private double evaluate() {
        double value = 0;
        if (nextToken.TokenType.equals("LPAREN")) {
            next();
            value = evaluate();
            next();
        } else {
            value = expr();
        }

        return value;
    }

    private double expr() {
        double value = term();

        while (nextToken.TokenType.equals("ADD") || nextToken.TokenType.equals("SUB")) {
            Tokens prev = nextToken;
            next();
            if (prev.TokenType.equals("ADD")) value += term(); else value -= term();
        }

        return value;
    }

    private double term() {
        double value = factor();

        while (nextToken.TokenType.equals("MUL") || nextToken.TokenType.equals("DIV") || nextToken.TokenType.equals("POW")) {
            Tokens prev = nextToken;
            next();
            if (prev.TokenType.equals("MUL")) value *= factor(); else if (prev.TokenType.equals("DIV")) value /= factor(); else value = Math.pow(value, factor());
        }

        return value;
    }

    private double factor() {
        double value = 0;
        switch (nextToken.TokenType) {
            case "NUM": value = Integer.parseInt(nextToken.Token);
                next();
                break;
            case "NUM_DOUBLE": value = Double.parseDouble(nextToken.Token);
                next();
                break;
            case "ID": value = xval;
                next();
                break;
            case "SIN": next();
                value = Math.sin(evaluate());
                break;
            case "COS": next();
                value = Math.cos(evaluate());
                break;
            case "TAN": next();
                value = Math.tan(evaluate());
                break;
            case "LN":  next();
                value = Math.log(evaluate());
                break;
            case "ATAN":next();
                value = Math.atan(evaluate());
                break;
            case "ATAN2":   next();
                if (nextToken.TokenType.equals("LPAREN")) {
                    next();
                    double lvalue = evaluate();
                    if (nextToken.TokenType.equals("COMMA")) {
                        next();
                        double rvalue = evaluate();
                        if (nextToken.TokenType.equals("RPAREN")) {
                            next();
                            value = Math.atan2(lvalue, rvalue);
                        } else {
                            throw new Error("Invalid Syntax!");
                        }
                    } else {
                        throw new Error("Invalid Syntax!");
                    }
                }
                break;
            case "LPAREN":  next();
                value = evaluate();
                if (nextToken.TokenType.equals("RPAREN")) next(); else throw new Error("Invalid Syntax!");
                break;
            default: throw new Error("Invalid Syntax!");

        }
        return value;
    }
}
