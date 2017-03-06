
import java.io.InputStream;
import java.io.InputStreamReader;

class DecimalParser {
  public static void main(String args[]) throws Exception {
	if (args.length==0) {
          	System.out.println("Usage: java NumberParser value");
		System.exit(1);
        }
        double val = MyParseDecimal(args[0]);
	System.out.println("Value="+val);
  }

  enum StateDecimal { SSTART, SINTEGER, SDECIMAL, SEND };

  public static double MyParseDecimal(String s) throws Exception {

    StateDecimal state;

    state = StateDecimal.SSTART;

    int i = 0;
    double divider = 10;
    double value = 0;

    while ( i < s.length() && state != StateDecimal.SEND) {
	char ch = s.charAt(i);
      	switch (state) {
        	case SSTART:
			if (Character.isDigit(ch)) {
				state = StateDecimal.SINTEGER;
				value = Character.getNumericValue(ch);
				i++;
			}
			else {
				throw new Exception("Bad format");
			}
			break;
		case SINTEGER:
			if (Character.isDigit(ch)) {
                                state = StateDecimal.SINTEGER;
                                value = 10.0*value + Character.getNumericValue(ch);
                                i++;
                        }
			else if (ch == '.') {
				state = StateDecimal.SDECIMAL;
				i++;
			}
			break;
		case SDECIMAL:
			if (Character.isDigit(ch)) {
                                value = value + Character.getNumericValue(ch)/divider;
				divider = divider * 10;
                                i++;
                        }
			else {
				throw new Exception("Bad format");
			}
			break;
	}

    }

    return value;


  }

}
