
public class Main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		String fileIn = "teste2.txt";
		ILParser ilParser = new ILParser();
		ILProgram program = ilParser.parse(fileIn);
		program.dump();
	}

}
