/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface.Forms;

public class FootSetup extends javax.swing.JDialog {
    private Program.Gate gate;

    public FootSetup(Program.Gate gate) {
        super(Window.w, true);
        this.gate = gate;
        initComponents();
        setLocationRelativeTo(Window.w);
        setIconImage((new javax.swing.ImageIcon(getClass().getResource("/Icons/icon.png"))).getImage());
        InputSpinner.setValue( gate.getInputs().size() );
        OutputSpinner.setValue( gate.getOutputs().size() );
        setVisible(true);
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        InputSpinner = new javax.swing.JSpinner();
        OutputSpinner = new javax.swing.JSpinner();
        Title = new javax.swing.JLabel();
        Inputs = new javax.swing.JLabel();
        Outputs = new javax.swing.JLabel();
        Separator = new javax.swing.JSeparator();
        Button = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Gate setup");
        setLocationByPlatform(true);
        setModal(true);
        setName("FootSetup"); // NOI18N
        setResizable(false);

        Title.setText("Number of inputs and outputs");

        Inputs.setText("Number of inputs:");

        Outputs.setText("Number of outputs:");

        Button.setText("OK");
        Button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(Title)
                            .addGroup(layout.createSequentialGroup()
                                .addGap(10, 10, 10)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(Inputs)
                                    .addComponent(Outputs))
                                .addGap(41, 41, 41)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                                    .addComponent(OutputSpinner)
                                    .addComponent(InputSpinner, javax.swing.GroupLayout.DEFAULT_SIZE, 44, Short.MAX_VALUE)))
                            .addComponent(Separator, javax.swing.GroupLayout.PREFERRED_SIZE, 200, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(69, 69, 69)
                        .addComponent(Button, javax.swing.GroupLayout.DEFAULT_SIZE, 84, Short.MAX_VALUE)
                        .addGap(67, 67, 67)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(Title)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(Separator, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(Inputs)
                    .addComponent(InputSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(Outputs)
                    .addComponent(OutputSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addComponent(Button)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void ButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ButtonActionPerformed
        int inputNum, outputNum;
        inputNum = (Integer)InputSpinner.getValue();
        outputNum = (Integer)OutputSpinner.getValue();
        if ( inputNum < 0 || outputNum < 0 ) {
            Message.dropError("Too low value :(");
            return;
        }
        if ( inputNum > 10 || outputNum > 10 ) {
            Message.dropError("Too high value :(");
            return;
        }
        gate.setInputNum( inputNum );
        gate.setOutputNum( outputNum );
        dispose();
    }//GEN-LAST:event_ButtonActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton Button;
    private javax.swing.JSpinner InputSpinner;
    private javax.swing.JLabel Inputs;
    private javax.swing.JSpinner OutputSpinner;
    private javax.swing.JLabel Outputs;
    private javax.swing.JSeparator Separator;
    private javax.swing.JLabel Title;
    // End of variables declaration//GEN-END:variables

}
