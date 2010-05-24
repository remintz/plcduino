import java.util.ArrayList;


public class ILProgram {
	private ArrayList <Instruction> instructionList;
	
	public ILProgram() {
		instructionList = new ArrayList<Instruction>();
	}
	
	public void add(Instruction i) {
		instructionList.add(i);
	}
	
	public void dump() {
		for (Instruction i: this.instructionList) {
			i.dump();
		}
	}
}
