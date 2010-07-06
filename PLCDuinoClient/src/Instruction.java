import java.util.ArrayList;


public class Instruction {
	private int operator;
	private int modifier;
	private int operand;
	private int byt;
	private int bit;
	
	public Instruction(int operator, int modifier, int operand, int byt, int bit) {
		this.operator = operator;
		this.modifier = modifier;
		this.operand = operand;
		this.byt = byt;
		this.bit = bit;
	}

	public void dump() {
		System.out.printf("%d\n", this.operator);
		System.out.printf("%d\n", this.modifier);
		System.out.printf("%d\n", this.operand);
		System.out.printf("%d\n", this.byt);
		System.out.printf("%d\n", this.bit);
	}

	public ArrayList<Integer> toList() {
		ArrayList<Integer> ret = new ArrayList<Integer>();
		ret.add(this.operator);
		ret.add(this.modifier);
		ret.add(this.operand);
		ret.add(this.byt);
		ret.add(this.bit);
		return ret;
	};

	//--- Modifiers
	public static final int IL_NOP 	= 0;
	public static final int IL_PUSH = 2;		// '('
	public static final int IL_NEG 	= 1;		// '!'
	public static final int IL_NORM = 3;		// ' '
	public static final int IL_COND = 4;		// '?'
	
	public static final int IL_CAL = 0;

	public enum IlOpCodes {
		//no operand
//		IL_END		(0),			//END
		IL_POP		(1),			//)
		IL_RET		(2),			//RET
		//arithmetic LABEL
		IL_JMP		(3),			//JMP
		//subroutine call (unimplemented)
		IL_CAL		(4),			//CAL
		//--------------- all operators below support arguments
		//boolean, no modifier
		IL_SET		(5),			//S
		IL_RESET	(6),		//R
		//boolean, all modifiers
		IL_AND		(7),		 	//AND
		IL_OR		(8),			//OR
		IL_XOR		(9),			//XOR
		IL_ANDN		(10),
		IL_ORN		(11),
		IL_XORN		(12),
		//any operand, only negation
		IL_LD		(13),			//LD
		IL_ST		(14),			//ST
		IL_LDN		(15),			//LDN
		IL_STN		(16),			//STN
		//any operand, only push
		IL_ADD		(17),
		IL_SUB		(18),
		IL_MUL		(19),
		IL_DIV		(20),
		IL_GT		(21),
		IL_GE		(22),
		IL_EQ		(23),
		IL_NE		(24),
		IL_LT		(25),
		IL_LE		(26),
		IL_LAST		(27);			// last OP... flag
		
		private final int code;
		private IlOpCodes(int code) {
			this.code = code;
		}
		public int getCode() {
			return code;
		}
		
	}

	
}
