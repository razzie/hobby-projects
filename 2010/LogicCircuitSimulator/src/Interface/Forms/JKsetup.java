/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface.Forms;

public class JKsetup extends javax.swing.JDialog {
    private Program.Gate gate;

    public JKsetup(Program.Gate gate) {
        super(Window.w, true);
        this.gate = gate;
        initComponents();
        setLocationRelativeTo(Window.w);
        setIconImage((new javax.swing.ImageIcon(getClass().getResource("/Icons/icon.png"))).getImage());
        Jsetup.setSelected( gate.getInputs().get(0).state() );
        Ksetup.setSelected( gate.getInputs().get(2).state() );
        setVisible(true);
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        Title = new javax.swing.JLabel();
        Separator = new javax.swing.JSeparator();
        Button = new javax.swing.JButton();
        Jsetup = new javax.swing.JCheckBox();
        Ksetup = new javax.swing.JCheckBox();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("JK flip-flop setup");
        setLocationByPlatform(true);
        setModal(true);
        setName("JKsetup"); // NOI18N
        setResizable(false);

        Title.setText("JK flip-flop setup");

        Button.setText("OK");
        Button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ButtonActionPerformed(evt);
            }
        });

        Jsetup.setText("J input powered");
        Jsetup.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                JsetupActionPerformed(evt);
            }
        });

        Ksetup.setText("K input powered");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(Jsetup))
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(Ksetup))
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(Separator, javax.swing.GroupLayout.PREFERRED_SIZE, 160, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(Title)))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(52, 52, 52)
                        .addComponent(Button, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addGap(43, 43, 43)))
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
                .addComponent(Jsetup)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(Ksetup)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 22, Short.MAX_VALUE)
                .addComponent(Button)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void ButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ButtonActionPerformed
        (gate.getInputs().get(0)).state( Jsetup.isSelected() );
        (gate.getInputs().get(2)).state( Ksetup.isSelected() );
        dispose();
    }//GEN-LAST:event_ButtonActionPerformed

    private void JsetupActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_JsetupActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_JsetupActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton Button;
    private javax.swing.JCheckBox Jsetup;
    private javax.swing.JCheckBox Ksetup;
    private javax.swing.JSeparator Separator;
    private javax.swing.JLabel Title;
    // End of variables declaration//GEN-END:variables

}
