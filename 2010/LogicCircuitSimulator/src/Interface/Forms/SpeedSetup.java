/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface.Forms;

public class SpeedSetup extends javax.swing.JDialog {
    private javax.swing.Timer timer;

    public SpeedSetup(javax.swing.Timer timer) {
        super(Window.w, true);
        this.timer = timer;
        int spd = 1000 / timer.getDelay();
        initComponents();
        setLocationRelativeTo(Window.w);
        setIconImage((new javax.swing.ImageIcon(getClass().getResource("/Icons/icon.png"))).getImage());
        Speed.setValue(spd);
        Current.setText(spd+" Hz");
        setVisible(true);
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        Panel = new javax.swing.JPanel();
        Speed = new javax.swing.JSlider();
        Min = new javax.swing.JLabel();
        Max = new javax.swing.JLabel();
        Current = new javax.swing.JLabel();
        Apply = new javax.swing.JButton();
        Use = new javax.swing.JButton();
        Cancel = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Speed setup");
        setLocationByPlatform(true);
        setModal(true);
        setName("SpeedSetup"); // NOI18N
        setResizable(false);

        Panel.setBorder(javax.swing.BorderFactory.createTitledBorder("Speed (default: 20 Hz)"));

        Speed.setMinimum(1);
        Speed.setMaximum(100);
        Speed.setValue(20);
        Speed.setFocusable(false);
        Speed.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                SpeedStateChanged(evt);
            }
        });

        Min.setText("1 Hz");

        Max.setText("100 Hz");

        Current.setFont(new java.awt.Font("Tahoma", 1, 11)); // NOI18N
        Current.setForeground(new java.awt.Color(0, 0, 255));
        Current.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        Current.setText("Current");

        javax.swing.GroupLayout PanelLayout = new javax.swing.GroupLayout(Panel);
        Panel.setLayout(PanelLayout);
        PanelLayout.setHorizontalGroup(
            PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(PanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(PanelLayout.createSequentialGroup()
                        .addComponent(Speed, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addContainerGap())
                    .addGroup(PanelLayout.createSequentialGroup()
                        .addComponent(Min)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(Current)
                        .addGap(49, 49, 49)
                        .addComponent(Max)
                        .addGap(16, 16, 16))))
        );
        PanelLayout.setVerticalGroup(
            PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(PanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(PanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(Min)
                    .addComponent(Max)
                    .addComponent(Current))
                .addGap(18, 18, 18)
                .addComponent(Speed, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        Apply.setText("OK");
        Apply.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                ApplyActionPerformed(evt);
            }
        });

        Use.setText("Apply");
        Use.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                UseActionPerformed(evt);
            }
        });

        Cancel.setText("Cancel");
        Cancel.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CancelActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(Apply, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(Use, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(Cancel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addComponent(Panel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(Panel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(Use)
                    .addComponent(Cancel)
                    .addComponent(Apply))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void ApplyActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_ApplyActionPerformed
        timer.setDelay( 1000 / Speed.getValue() );
        dispose();
    }//GEN-LAST:event_ApplyActionPerformed

    private void CancelActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_CancelActionPerformed
        dispose();
    }//GEN-LAST:event_CancelActionPerformed

    private void SpeedStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_SpeedStateChanged
        int spd = Speed.getValue();
        Speed.setToolTipText(spd+" Hz");
        Current.setText(spd+" Hz");
    }//GEN-LAST:event_SpeedStateChanged

    private void UseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_UseActionPerformed
        timer.setDelay( 1000 / Speed.getValue() );
    }//GEN-LAST:event_UseActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton Apply;
    private javax.swing.JButton Cancel;
    private javax.swing.JLabel Current;
    private javax.swing.JLabel Max;
    private javax.swing.JLabel Min;
    private javax.swing.JPanel Panel;
    private javax.swing.JSlider Speed;
    private javax.swing.JButton Use;
    // End of variables declaration//GEN-END:variables

}
