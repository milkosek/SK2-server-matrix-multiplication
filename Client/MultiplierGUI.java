package Client;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.Socket;
import java.util.ArrayList;

public class MultiplierGUI implements ActionListener {

    JFrame f;
    JTextField tf;
    JTextField tfIp;
    JButton proc;
    JLabel l;
    JLabel l1 = new JLabel("Matrix 1");
    JLabel l2 = new JLabel("Matrix 2");
    ArrayList<JTextField> FirstMatrixFields;
    ArrayList<JTextField> SecondMatrixFields;
    ArrayList<JTextField> FinalMatrix;
    JButton calc = new JButton("Multiply");
    JLabel NFE = null;
    int size;
    int lastY;
    String[] ipList;
    PrintWriter out;
    BufferedReader in;

    public MultiplierGUI() {
        f = new JFrame();
        l = new JLabel("Matrix size:");
        l.setBounds(20, 70, 100, 30);
        tf = new JTextField();
        tf.setBounds(20, 95, 30, 20);
        proc = new JButton("Proceed");
        proc.setBounds(20, 120, 100, 25);
        proc.addActionListener(this);
        f.add(l);
        f.add(tf);
        f.add(proc);
        l = new JLabel("Server IP & port (x.x.x.x:x):");
        l.setBounds(20, 20, 170, 30);
        tfIp = new JTextField();
        tfIp.setBounds(20, 45, 100, 20);
        f.add(l);
        f.add(tfIp);
        f.setSize(300, 500);
        f.setLayout(null);
        f.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        f.setVisible(true);
    }


    public void actionPerformed(ActionEvent e){
        if (e.getSource() == proc) {
            if (NFE != null){
                f.remove(NFE);
            }
            String ip = tfIp.getText();
            ipList = ip.split(":");
            if(ipList.length != 2){
                NFE = new JLabel("Invalid IP address!");
                NFE.setBounds(120, 45, 120, 20);
                f.add(NFE);
                SwingUtilities.updateComponentTreeUI(f);
                return;
            }
            if(!ipList[0].matches("localhost")) {
                for (String s : ipList[0].split("\\.")) {
                    if (!s.matches("[0-9]+") || s.length() > 3 || Integer.parseInt(s) > 255) {
                        NFE = new JLabel("Invalid IP address!");
                        NFE.setBounds(120, 45, 120, 20);
                        f.add(NFE);
                        SwingUtilities.updateComponentTreeUI(f);
                        return;
                    }
                }
            }
            if(!ipList[1].matches("[0-9]+")){
                NFE = new JLabel("Invalid IP address!");
                NFE.setBounds(120, 45, 120, 20);
                f.add(NFE);
                SwingUtilities.updateComponentTreeUI(f);
                return;
            }
            try {
                size = Integer.parseInt(tf.getText());
                if (size < 1) {
                    throw new NumberFormatException();
                }
            } catch(NumberFormatException exception) {
                NFE = new JLabel("Wrong matrix size!");
                NFE.setBounds(50, 95, 120, 20);
                f.add(NFE);
                SwingUtilities.updateComponentTreeUI(f);
                return;
            }
            int x, y;
            if (FirstMatrixFields != null) {
                for (int i=0; i<FirstMatrixFields.size(); ++i) {
                    f.remove(FirstMatrixFields.get(i));
                    f.remove(SecondMatrixFields.get(i));
                }
            }
            if (FinalMatrix != null){
                for (JTextField field : FinalMatrix){
                    f.remove(field);
                }
            }
            FirstMatrixFields = new ArrayList<>();
            SecondMatrixFields = new ArrayList<>();
            FinalMatrix = new ArrayList<>();
            x = 20;
            y = 200;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    JTextField field1 = new JTextField();
                    JTextField field2 = new JTextField();
                    field1.setBounds(x, y, 30, 20);
                    field2.setBounds(x+(50*size), y, 30, 20);
                    FirstMatrixFields.add(field1);
                    SecondMatrixFields.add(field2);
                    f.add(field1);
                    f.add(field2);
                    x += 35;
                }
                x = 20;
                y += 25;
            }
            l1.setBounds(20, 120, 100, 30);
            l2.setBounds(20+(50*size), 120, 100, 30);
            calc.setBounds(20, y+30, 100, 25);
            lastY = y+30;
            calc.addActionListener(this);
            f.add(l1);
            f.add(l2);
            f.add(calc);
            SwingUtilities.updateComponentTreeUI(f);
        }

        else if (e.getSource() == calc){
            if (NFE != null){
                f.remove(NFE);
            }
            if (FinalMatrix != null){
                for (JTextField field : FinalMatrix){
                    f.remove(field);
                }
            }
            ArrayList<Double> Matrix1 = new ArrayList<>();
            ArrayList<Double> Matrix2 = new ArrayList<>();
            FinalMatrix = new ArrayList<>();
            for(int i=0; i<size*size; ++i){
                try {
                    Matrix1.add(Double.parseDouble(FirstMatrixFields.get(i).getText()));
                } catch(NumberFormatException exception){
                    NFE = new JLabel(String.format("Wrong input at Matrix1[%d][%d]!", i%size, i/size));
                    NFE.setBounds(60,  145, 200, 20);
                    f.add(NFE);
                    SwingUtilities.updateComponentTreeUI(f);
                    return;
                }
                try {
                    Matrix2.add(Double.parseDouble(SecondMatrixFields.get(i).getText()));
                } catch(NumberFormatException exception){
                    NFE = new JLabel(String.format("Wrong input at Matrix2[%d][%d]!", i%size, i/size));
                    NFE.setBounds(40, 145, 200, 20);
                    f.add(NFE);
                    SwingUtilities.updateComponentTreeUI(f);
                    return;
                }
            }

            try {
                Socket clSocket = new Socket(ipList[0], Integer.parseInt(ipList[1]));
                out = new PrintWriter(clSocket.getOutputStream());
                in = new BufferedReader(new InputStreamReader(clSocket.getInputStream()));
                String msg;
                msg = ":"+size+":";
                out.print(msg);
                if(!in.readLine().equals(msg)){
                    clSocket.close();
                    out.close();
                    in.close();
                    JOptionPane.showMessageDialog(null,"There's been an error. Please try again");
                }
                for(int i=0; i<size*size; ++i){
                    msg = ";"+Matrix1.get(i)+","+Matrix2.get(i)+";";
                    out.print(msg);
                }
            } catch (IOException ex) {
                JOptionPane.showMessageDialog(null,"There's been an error. Please try again");
            }


            try {
                String msg = in.readLine();
                String[] mat = msg.split(";");
                int x = 20;
                int y = lastY+40;
                int z = 0;
                for (int i = 0; i < size; ++i) {
                    for (int j = 0; j < size; ++j) {
                        JTextField field = new JTextField();
                        field.setBounds(x, y, 30, 20);
                        field.setEditable(false);
                        field.setText(mat[z++]);
                        f.add(field);
                        FinalMatrix.add(field);
                        x += 35;
                    }
                    x = 20;
                    y += 25;
                }
            } catch (IOException ex) {
                JOptionPane.showMessageDialog(null,"There's been an error. Please try again");
            }
            /*int x = 20;
            int y = lastY+40;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    JTextField field = new JTextField();
                    field.setBounds(x, y, 30, 20);
                    field.setEditable(false);
                    field.setText("0");
                    f.add(field);
                    FinalMatrix.add(field);
                    x += 35;
                }
                x = 20;
                y += 25;
            }*/
            SwingUtilities.updateComponentTreeUI(f);
        }
    }

    public static void main(String[] args) {
        new MultiplierGUI();
    }
}
