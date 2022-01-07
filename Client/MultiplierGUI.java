package Client;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

public class MultiplierGUI implements ActionListener {

    JFrame f;
    JTextField tf;
    JButton proc;
    JLabel l;
    JLabel l1 = new JLabel("Matrix 1");
    JLabel l2 = new JLabel("Matrix 2");
    ArrayList<JTextField> FirstMatrixFields;
    ArrayList<JTextField> SecondMatrixFields;
    JButton calc = new JButton("Multiply");
    JLabel NFE = null;
    int size;
    int lastY;

    public MultiplierGUI() {
        f = new JFrame();
        l = new JLabel("Matrix size:");
        l.setBounds(20, 20, 100, 30);
        tf = new JTextField();
        tf.setBounds(20, 45, 30, 20);
        proc = new JButton("Proceed");
        proc.setBounds(20, 70, 100, 25);
        proc.addActionListener(this);
        f.add(l);
        f.add(tf);
        f.add(proc);
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
            try {
                size = Integer.parseInt(tf.getText());
                if (size < 1) {
                    throw new NumberFormatException();
                }
            } catch(NumberFormatException exception) {
                NFE = new JLabel("Wrong input!");
                NFE.setBounds(55, 45, 120, 20);
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
            FirstMatrixFields = new ArrayList<>();
            SecondMatrixFields = new ArrayList<>();
            x = 20;
            y = 150;
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
            ArrayList<Double> Matrix1 = new ArrayList<>();
            ArrayList<Double> Matrix2 = new ArrayList<>();

            for(int i=0; i<size*size; ++i){
                try {
                    Matrix1.add(Double.parseDouble(FirstMatrixFields.get(i).getText()));
                } catch(NumberFormatException exception){
                    NFE = new JLabel(String.format("Wrong input at Matrix1[%d][%d]!", i%size, i/size));
                    NFE.setBounds(40,  95, 200, 20);
                    f.add(NFE);
                    SwingUtilities.updateComponentTreeUI(f);
                    return;
                }
                try {
                    Matrix2.add(Double.parseDouble(SecondMatrixFields.get(i).getText()));
                } catch(NumberFormatException exception){
                    NFE = new JLabel(String.format("Wrong input at Matrix2[%d][%d]!", i%size, i/size));
                    NFE.setBounds(40, 95, 200, 20);
                    f.add(NFE);
                    SwingUtilities.updateComponentTreeUI(f);
                    return;
                }
            }
            int x = 20;
            int y = lastY+40;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    JTextField field = new JTextField();
                    field.setBounds(x, y, 30, 20);
                    field.setEditable(false);
                    field.setText("0");
                    f.add(field);
                    x += 35;
                }
                x = 20;
                y += 25;
            }
            SwingUtilities.updateComponentTreeUI(f);
        }
    }

    public static void main(String[] args) {
        new MultiplierGUI();
    }
}
