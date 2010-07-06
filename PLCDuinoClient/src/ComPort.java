import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.TooManyListenersException;
import gnu.io.*;

public class ComPort {
	InputStream in;
	OutputStream out;
	SerialPort serialPort;
	StringBuffer inBuf = new StringBuffer();
	void connect (String portName) throws Exception
    {
		CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
        if (portIdentifier.isCurrentlyOwned()) {
            System.out.println("Error: Port is currently in use");
            System.exit(1);
        }
        else {
            CommPort commPort = portIdentifier.open(this.getClass().getName(),2000);
            if (commPort instanceof SerialPort) {
                serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(9600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
                in = serialPort.getInputStream();
                out = serialPort.getOutputStream();
            }
            else {
                System.out.println("Error: Only serial ports are handled.");
            }
        }     
	}

	public void disconnect() {
		try {
			in.close();
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		this.serialPort.close();
	}
	
	public void sendString(String string) throws IOException {
//		System.out.println("Send: "+string);
		this.out.write(string.getBytes());
	}

	public String getString() throws IOException {
		int len = -1;
		byte[] buffer = new byte[255];
		while ((len = this.in.read(buffer)) == -1);
		return new String(buffer,0,len);
	}
}
