/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface.Forms;

public class NewProjectName extends javax.swing.JDialog {
    private Program.Project p;
    private javax.swing.JTabbedPane Workspace;

    public NewProjectName(Program.Project p, javax.swing.JTabbedPane Workspace) {
        super(Window.w, true);
        this.p = p;
        this.Workspace = Workspace;
        initComponents();
        setLocationRelativeTo(Window.w);
        newProjectText.setText( p.name() );
        setVisible(true);
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jTextField1 = new javax.swing.JTextField();
        newProjectLabel = new javax.swing.JLabel();
        newProjectText = new javax.swing.JTextField();
        newProjectOk = new javax.swing.JButton();
        newProjectCancel = new javax.swing.JButton();

        jTextField1.setText("jTextField1");

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Project name");
        setLocationByPlatform(true);
        setModal(true);

        newProjectLabel.setText("Project name:");

        newProjectOk.setText("Save");
        newProjectOk.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                newProjectOkActionPerformed(evt);
            }
        });

        newProjectCancel.setText("Cancel");
        newProjectCancel.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                newProjectCancelActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(newProjectLabel)
                    .addComponent(newProjectText, javax.swing.GroupLayout.DEFAULT_SIZE, 265, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addComponent(newProjectOk)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(newProjectCancel)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(newProjectLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(newProjectText, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(newProjectCancel)
                    .addComponent(newProjectOk))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void newProjectOkActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_newProjectOkActionPerformed
        String name = newProjectText.getText();
        p.rename( name );
        Workspace.setTitleAt( Workspace.getSelectedIndex() , name);
        dispose();
    }//GEN-LAST:event_newProjectOkActionPerformed

    private void newProjectCancelActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_newProjectCancelActionPerformed
        dispose();
    }//GEN-LAST:event_newProjectCancelActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JTextField jTextField1;
    private javax.swing.JButton newProjectCancel;
    private javax.swing.JLabel newProjectLabel;
    private javax.swing.JButton newProjectOk;
    private javax.swing.JTextField newProjectText;
    // End of variables declaration//GEN-END:variables

}
