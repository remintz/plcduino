import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;


public class ILParser {
	private boolean isVerbose;
	private static final int ERROR = -1;
	private static final int CH_ERR_BADCHAR = 255;
	private static final String[] IL_COMMANDS = {
		//--- same order as IL_OPCODES enum
		")",
		"RET",
		"JMP",
		"CAL",
		"S",
		"R",
		"AND",
		"OR",
		"XOR",
		"ANDN",
		"ORN",
		"XORN",
		"LD",
		"ST",
		"LDN",
		"STN",
		"ADD",
		"SUB",
		"MUL",
		"DIV",
		"GT",
		"GE",
		"EQ",
		"NE",
		"LT",
		"LE"
	};

	class ParserException extends Exception {
		private static final long serialVersionUID = 1L;
		public ParserException(String message) {
			super(message);
		}
	}

	//--- Ladder character codes
	int extract_number(String line, int start)
	{
		if (start >= line.length())
			return ERROR;
		int i, n;
		//starting @ position start
		n = 0;
		i = start;
		while (Character.isDigit(line.charAt(i))) {
			n = 10 * n + (line.charAt(i) - '0');
			i++;
			if (i >= line.length()) break;
		}
		if (i == start)
			//no digits read
			return ERROR;
		return n;
		//return number read or error
	}

	int read_char(String line, int idx)
	{
		//read ONE character from line[idx] parse grammatically
		int r = 0;
		if (idx > line.length())
			return ERROR;
		if (Character.isDigit(line.charAt(idx)))
			return line.charAt(idx) - '0';
		switch (line.charAt(idx)) {
		case 'I':
		case 'Q':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'C':
		case 'D':
		case 'E':
		case 'T':
		case 'U':
		case 'S':
		case 'R':
		case 'X':
		case 'A':
			r = line.charAt(idx);
			break;
		default:
			r = CH_ERR_BADCHAR;//error
		break;
		}
		//return value or error
		return r;
	}

	Instruction parse_il_line(String line) throws ParserException
	{//    line format:[label:]<operator>[<modifier>[%<operand><byte>[/<bit>]]|<label>][;comment]
		String buf;
		int i, j, byt = 0, bit = 0;
		String label_buf;
		int modifier, operand = 0, operator;

		if (isVerbose)
			System.out.printf("Instr: %s\n",line);

		//--- 1. read up to ';' or \n and truncate comments
		i = 0;
		j = line.indexOf(';');
		if (j >= 0)
			line = line.substring(0, j);
		if (line.length() == 0) return null;
		//--- 2. find last ':', trunc up to there, store label.
		j = line.lastIndexOf(':');
		if (j > 0)
		{
			i = 0;
			label_buf = line.substring(0, j);
			buf = line.substring(j);
			if (isVerbose)
				System.out.printf("label:%s\n",label_buf);
		}
		else
			buf = line;

		// skip blanks and tabs
		i = 0;
		while (buf.charAt(i) == ' ' || buf.charAt(i) == '\t') {
			i++;
			if (i >= line.length()) return null;
		}
		buf = buf.substring(i);
		//--- get operator

		//---3. find first ' ','!','('. store modifier (0 if not found)
		modifier = Instruction.IL_NOP;
		if ((j = buf.indexOf('(')) >= 0)
			modifier = Instruction.IL_PUSH;	// push stack
		else
		{
			if ((j = buf.indexOf('!')) >= 0)
				modifier = Instruction.IL_NEG;	// negate
			else
			{
				if ((j = buf.indexOf('?')) >= 0)
					modifier = Instruction.IL_COND;	// conditional branch
				else
				{
					if ((j = buf.indexOf(' ')) >= 0)
						modifier = Instruction.IL_NORM;	// normal
				}
			}
		}
		if (isVerbose)
			System.out.printf("modifier:%d\n",modifier);

		String bufOp = buf;
		//---4. read operator from beginning to modifier. check if invalid, (return error)
		if (j >= 0) {
			bufOp = buf.substring(0, j);
			buf = buf.substring(j+1);
		}
		for (i = 0; i < IL_COMMANDS.length; i++) {
			if (bufOp.equalsIgnoreCase(IL_COMMANDS[i])) break;
		}
		if (i >= IL_COMMANDS.length)
			throw new ParserException("ERR_BADOPERATOR");
		operator = i;
		if (isVerbose)
			System.out.printf("operator:%s ",IL_COMMANDS[operator]);
		if (isVerbose)
			System.out.printf("(%d)\n",operator);
		if (operator > Instruction.IL_CAL) {
			//5. if operator supports arguments, find first '%'. (special case: JMP (string). if not found return error
			if ((j = buf.indexOf('%')) < 0)
				throw new ParserException("ERR_BADCHAR");
			buf = buf.substring(j+1);
			//6.  read first non-numeric char after '%'. if not found return error. store operand. check if invalid (return error).
			operand = 0;
			if (Character.isLetter(buf.charAt(0)))
				operand = read_char(buf,0);
			else
				throw new ParserException("ERR_BADOPERAND");
			if (operand == CH_ERR_BADCHAR)
				throw new ParserException("ERR_BADOPERAND");
			if (isVerbose)
				System.out.printf("operand:%d\n",operand);

			//7.  read first numeric chars after operand. store byte. if not found or out of range, return error.
			byt = extract_number(buf,1);
			if (byt < 0)
				throw new ParserException("ERR_BADINDEX");
			if (isVerbose)
				System.out.printf("byte:%d\n",byt);

			//8.  find '/'. if not found truncate, return.
			bit = 64;
			if ((j = buf.indexOf('/')) > 0)
			{
				buf = buf.substring(j);
				if (!Character.isDigit(buf.charAt(1)) || buf.charAt(1) > '7')
					throw new ParserException("ERR_BADINDEX");
				else
					bit = buf.charAt(1) - '0';
				if (isVerbose)
					System.out.printf ("bit:%d\n",bit);
			}
			//9.  if found, read next char. if not found, or not numeric, or >7, return error. store bit.
		}
		//			else
		//			{
		//				if (operator==IL_JMP)
		//				{
		//					str = strchr(buf, ' ');
		//					if (!str)
		//						return ERR_BADOPERAND;
		//					strcpy(op->jmpLabel, str+1);
		//				}
		//			}
		//			strcpy(op->lineLabel, label_buf);
		Instruction op = new Instruction(operator, modifier, operand, byt, bit);
		return op;
	}

	public ILProgram parse(String fileIn, boolean verbose) {
		isVerbose = verbose;
		File inputFile = new File(fileIn);
		BufferedReader input = null;
		try {
			input = new BufferedReader(new FileReader(inputFile));
		} catch (FileNotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		String line = null;
		ILProgram p = new ILProgram();
		Instruction op;
		try {
			try {
				while ((line = input.readLine()) != null) {
					try {
						op = parse_il_line(line);
						if (op != null) {
							p.add(op);
						}
					}
					catch (ParserException e) {
						System.out.printf("ABORT: %s\n", e.getMessage());
						return null;
					}
				}
			} finally {
				input.close();
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return p;
	}
}
