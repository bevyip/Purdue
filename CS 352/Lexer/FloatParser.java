import static java.lang.Math.pow;
import java.io.InputStream;
import java.io.InputStreamReader;

class FloatParser {
	public static void main(String args[]) throws Exception {
		if (args.length==0) {
			System.out.println("Usage: java NumberParser value");
			System.exit(1);
		}
		double val = MyParseFloat(args[0]);
		System.out.println("Value="+val);
	}

	enum StateFloat { SA, SB, SC, SD, SE, SF, SG, SH };

	public static double MyParseFloat(String s) throws Exception {
		// Using the code in DecimalParser.java write a finite state
		// machine that parses a floating point number of the form
		//             [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?
		StateFloat state;

		state = StateFloat.SA;

		int i = 0;
		Boolean neg = false;
		Boolean nege = false;
		double divider = 10;
		double value = 0;
		int exponent = 0;

	while (i < s.length() && (state != StateFloat.SC || state != StateFloat.SF || state != StateFloat.SH)) {
			char ch = s.charAt(i);
			switch (state) {
				case SA:
					if (Character.isDigit(ch)) {
						state = StateFloat.SC;
						value = Character.getNumericValue(ch);
						i++;
					} else if (ch == '.') {
						state = StateFloat.SD;
						i++;
					} else if (ch == '+' || ch == '-') {
						state = StateFloat.SB;
						if (ch == '-') neg = true;
						i++;
					} else {
						throw new Exception("Bad Format");
					}
					break;
				case SB:
					if (Character.isDigit(ch)) {
						state = StateFloat.SC;
						if (neg) value = 10*value - Character.getNumericValue(ch);
						else value = 10*value + Character.getNumericValue(ch);
						i++;
					} else if (ch == '.') {
						state = StateFloat.SD;
						i++;
					} else {
						throw new Exception("Bad Format");
					}
					break;
				case SC:
					if (Character.isDigit(ch)) {
						if (neg) value = 10*value - Character.getNumericValue(ch);
						else value = 10*value + Character.getNumericValue(ch);
						i++;
					} else if (ch == '.') {
						state = StateFloat.SD;
						i++;
					} else if (ch == 'e' || ch == 'E') {
						state = StateFloat.SE;
						i++;
					} else {
						throw new Exception("Bad Format");
					}
					break;
				case SD:
					if (Character.isDigit(ch)) {
						state = StateFloat.SF;
						if (neg) value -= Character.getNumericValue(ch)/divider;
						else value += Character.getNumericValue(ch)/divider;
						divider *= 10;
						i++;
					} else {
						throw new Exception("Bad Format");
					}
					break;
				case SE:
					if (Character.isDigit(ch)) {
						state = StateFloat.SH;
						if (nege) exponent = 10*exponent - Character.getNumericValue(ch);
						else exponent = 10*exponent + Character.getNumericValue(ch);
						i++;
					} else if (ch == '+' || ch == '-') {
						state = StateFloat.SG;
						if (ch == '-') nege = true;
						i++;
					} else {
						throw new Exception("Bad Format");
					}
					break;
				case SF:
					if (Character.isDigit(ch)) {
						//Divider comes here
						if (neg) value -= Character.getNumericValue(ch)/divider;
						else value += Character.getNumericValue(ch)/divider;
						divider *= 10;
						i++;
					} else if (ch == 'e' || ch == 'E') {
						state = StateFloat.SE;
						i++;
					} else {
						throw new Exception("Bad Format");
					}
					break;
				case SG:
					if (Character.isDigit(ch)) {
						state = StateFloat.SH;
						if (nege) exponent = 10*exponent - Character.getNumericValue(ch);
						else exponent = 10*exponent + Character.getNumericValue(ch);
						i++;
					} else {
						throw new Exception("Bad Format");
					}
					break;
				case SH:
					if (Character.isDigit(ch)) {
						if (nege) exponent = 10*exponent - Character.getNumericValue(ch);
						else exponent = 10*exponent + Character.getNumericValue(ch);
						i++;
					} else {
						throw new Exception("Bad Format");
					}
			}
		}

		if (state != StateFloat.SC && state != StateFloat.SF && state != StateFloat.SH) throw new Exception("Bad Format");
		value = value * Math.pow(10, exponent);
		return value;
	}

}
