
public class Main {

	private static void usage() {
		System.out.println("Usage: PLCDuinoClient -f<filename> [-d] [-v] [-c<portaserial>]");
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("PLCduino Parser/Loader");
		String fileIn = "";
		String serialPort = "";
		Boolean dump = false;
		Boolean verbose = false;
		if (args.length < 1) {
			usage();
			return;
		}
		for (int i = 0; i < args.length; i++) { 
			if (args[i].startsWith("-f")) {
				fileIn = args[i].replace("-f", "");
			} else if (args[i].startsWith("-d")) {
				dump = true;
			} else if (args[i].startsWith("-v")) {
				dump = true;
			} else if (args[i].startsWith("-c")) {
				serialPort = args[i].replace("-c","");
			}
		}
		if (fileIn.isEmpty()) {
			usage();
			return;
		}
		ILParser ilParser = new ILParser();
		System.out.println("Parsing " + fileIn);
		ILProgram program = ilParser.parse(fileIn, verbose);
		System.out.println("Parse OK");
		if (serialPort.length() > 0) {
			Uploader m_Uploader = new Uploader(serialPort);
			m_Uploader.upload(program.toArray());
			System.out.println("Uploaded " + program.toArray().size() + " bytes.");
		}
		if (dump)
			program.dump();
	}


}
