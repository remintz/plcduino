import java.io.IOException;
import java.util.ArrayList;

public class Uploader {
	private ComPort serial;
	public Uploader(String serialPort) {
		serial = new ComPort();
		try {
			serial.connect(serialPort);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void dropUntil(String p_Find) {
		StringBuilder s = new StringBuilder();
		try {
			do {
				s.append(serial.getString());
//				System.out.println("S:" + s.toString());
			} while (s.indexOf(p_Find) < 0);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void upload(ArrayList<Integer> program) {
		String s;
		try {
			dropUntil(">");
			serial.sendString("l 20\r");
			dropUntil("stop.");
			for (Integer i: program) {
				s = i.toString();
				serial.sendString(s+"\r");
				dropUntil(s);
			}
		serial.sendString("256\r");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		serial.disconnect();
	}
}
