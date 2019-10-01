/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface.Forms;

public class DisplaySetup extends javax.swing.JDialog {
    private Program.Gates.Display display;

    public DisplaySetup(Program.Gates.Display display) {
        super(Window.w, true);
        this.display = display;
        initComponents();
        setLocationRelativeTo(Window.w);
        setIconImage((new javax.swing.ImageIcon(getClass().getResource("/Icons/icon.png"))).getImage());
        RowSpinner.setValue( display.getRows() );
        ColumnSpinner.setValue( display.getColumns() );
        setVisible(true);
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        RowSpinner = new javax.swing.JSpinner();
        ColumnSpinner = new javax.swing.JSpinner();
        Title = new javax.swing.JLabel();
        Inputs = new javax.swing.JLabel();
        Outputs = new javax.swing.JLabel();
        Separator = new javax.swing.JSeparator();
        Button = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Display setup");
        setLocationByPlatform(true);
        setModal(true);
        setName("FootSetup"); // NOI18N
        setResizable(false);

        Title.setText("Display setup");

        Inputs.setText("Number of rows:");

        Outputs.setText("Number of columns:");

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
                                    .addComponent(ColumnSpinner)
                                    .addComponent(RowSpinner, javax.swing.GroupLayout.DEFAULT_SIZE, 44, Short.MAX_VALUE)))
                            .addComponent(Separator, javax.swing.GroupLayout.PREFERRED_SIZE, 186, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(61, 61, 61)
                        .addComponent(Button, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addGap(65, 65, 65)))
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
                    .addComponent(RowSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(Outputs)
                    .addComponent(ColumnSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addComponent(Button)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void ButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ButtonActionPerformed
        int rows, columns;
        rows = (Integer)RowSpinner.getValue();
        columns = (Integer)ColumnSpinner.getValue();
        if ( rows < 1 || columns < 1 ) {
            Message.dropError("Too low value :(");
            return;
        }
        if ( rows > 40 || columns > 60 ) {
            Message.dropError("Too high value :(");
            return;
        }
        display.setSize(rows, columns);
        dispose();
    }//GEN-LAST:event_ButtonActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton Button;
    private javax.swing.JSpinner ColumnSpinner;
    private javax.swing.JLabel Inputs;
    private javax.swing.JLabel Outputs;
    private javax.swing.JSpinner RowSpinner;
    private javax.swing.JSeparator Separator;
    private javax.swing.JLabel Title;
    // End of variables declaration//GEN-END:variables

}
