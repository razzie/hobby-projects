/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

package Interface.Forms;

public class TimerSetup extends javax.swing.JDialog {
    private Program.Gates.Timer timer;

    public TimerSetup(Program.Gates.Timer timer) {
        super(Window.w, true);
        this.timer = timer;
        initComponents();
        setLocationRelativeTo(Window.w);
        setIconImage((new javax.swing.ImageIcon(getClass().getResource("/Icons/icon.png"))).getImage());
        OutputSpinner.setValue( timer.getOutputs().size() );
        int on = timer.getTimerOn(), off = timer.getTimerOff();
        TimerOnMs.setValue( on );
        TimerOffMs.setValue( off );
        if ( on == off && on >= 10 && on <= 500 ) {
            FreqLabel.setText( 500/on + " Hz");
            Freq.setValue( 500/on );
            Freq.setEnabled(true);
            TimerOnMs.setEnabled(false);
            TimerOffMs.setEnabled(false);
            ChangeMs.setSelected(false);
        }
        setVisible(true);
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        Title = new javax.swing.JLabel();
        Separator1 = new javax.swing.JSeparator();
        FreqText = new javax.swing.JLabel();
        FreqLabel = new javax.swing.JLabel();
        Freq = new javax.swing.JSlider();
        ChangeMs = new javax.swing.JCheckBox();
        TimerOn = new javax.swing.JLabel();
        TimerOff = new javax.swing.JLabel();
        TimerOnMs = new javax.swing.JSpinner();
        TimerOffMs = new javax.swing.JSpinner();
        Separator2 = new javax.swing.JSeparator();
        Outputs = new javax.swing.JLabel();
        OutputSpinner = new javax.swing.JSpinner();
        Button = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Timer setup");
        setLocationByPlatform(true);
        setModal(true);
        setName("FootSetup"); // NOI18N
        setResizable(false);

        Title.setText("Timer setup");

        FreqText.setText("Frequency:");

        FreqLabel.setFont(new java.awt.Font("Tahoma", 1, 11)); // NOI18N
        FreqLabel.setForeground(new java.awt.Color(0, 0, 255));
        FreqLabel.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        FreqLabel.setText("- Hz");

        Freq.setMaximum(50);
        Freq.setMinimum(1);
        Freq.setValue(1);
        Freq.setEnabled(false);
        Freq.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                FreqStateChanged(evt);
            }
        });

        ChangeMs.setSelected(true);
        ChangeMs.setText("Millisecond setup");
        ChangeMs.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                ChangeMsItemStateChanged(evt);
            }
        });

        TimerOn.setText("Powered: (ms.)");

        TimerOff.setText("Unpowered: (ms.)");

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
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(Title)
                        .addGap(159, 159, 159))
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(Separator2)
                            .addComponent(Freq, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                            .addComponent(Separator1, javax.swing.GroupLayout.Alignment.TRAILING)
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(ChangeMs)
                                    .addGroup(layout.createSequentialGroup()
                                        .addGap(10, 10, 10)
                                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addComponent(TimerOn)
                                            .addComponent(TimerOff)
                                            .addComponent(Outputs))))
                                .addGap(54, 54, 54)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addComponent(TimerOnMs, javax.swing.GroupLayout.PREFERRED_SIZE, 78, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(TimerOffMs, javax.swing.GroupLayout.PREFERRED_SIZE, 78, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(OutputSpinner, javax.swing.GroupLayout.PREFERRED_SIZE, 45, javax.swing.GroupLayout.PREFERRED_SIZE)))
                            .addGroup(layout.createSequentialGroup()
                                .addGap(8, 8, 8)
                                .addComponent(FreqText)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(FreqLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addContainerGap())))
            .addGroup(layout.createSequentialGroup()
                .addGap(91, 91, 91)
                .addComponent(Button, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE)
                .addGap(91, 91, 91))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(Title)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(Separator1, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(FreqText)
                    .addComponent(FreqLabel))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(Freq, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(ChangeMs)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(TimerOn)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(31, 31, 31)
                        .addComponent(TimerOff))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(TimerOnMs, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(TimerOffMs, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(11, 11, 11)
                .addComponent(Separator2, javax.swing.GroupLayout.PREFERRED_SIZE, 10, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
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
        int outputNum, msOn, msOff;
        outputNum = (Integer)OutputSpinner.getValue();
        msOn = (Integer)TimerOnMs.getValue();
        msOff = (Integer)TimerOffMs.getValue();
        if ( outputNum < 0 || msOn < 0 || msOff < 0 ) {
            Message.dropError("Too low value :(");
            return;
        }
        if ( outputNum > 10 ) {
            Message.dropError("Too many outputs :(");
            return;
        }
        timer.setOutputNum( outputNum );
        timer.setTimer( msOn , msOff );
        dispose();
    }//GEN-LAST:event_ButtonActionPerformed

    private void ChangeMsItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_ChangeMsItemStateChanged
        if ( evt.getStateChange() == java.awt.event.ItemEvent.SELECTED ) {
            FreqLabel.setText("- Hz");
            Freq.setEnabled(false);
            TimerOnMs.setEnabled(true);
            TimerOffMs.setEnabled(true);
        }
        if ( evt.getStateChange() == java.awt.event.ItemEvent.DESELECTED ) {
            Freq.setEnabled(true);
            TimerOnMs.setEnabled(false);
            TimerOffMs.setEnabled(false);
        }
    }//GEN-LAST:event_ChangeMsItemStateChanged

    private void FreqStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_FreqStateChanged
        int val = 500 / Freq.getValue();
        TimerOnMs.setValue(val);
        TimerOffMs.setValue(val);
        FreqLabel.setText(Freq.getValue()+" Hz");
    }//GEN-LAST:event_FreqStateChanged

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton Button;
    private javax.swing.JCheckBox ChangeMs;
    private javax.swing.JSlider Freq;
    private javax.swing.JLabel FreqLabel;
    private javax.swing.JLabel FreqText;
    private javax.swing.JSpinner OutputSpinner;
    private javax.swing.JLabel Outputs;
    private javax.swing.JSeparator Separator1;
    private javax.swing.JSeparator Separator2;
    private javax.swing.JLabel TimerOff;
    private javax.swing.JSpinner TimerOffMs;
    private javax.swing.JLabel TimerOn;
    private javax.swing.JSpinner TimerOnMs;
    private javax.swing.JLabel Title;
    // End of variables declaration//GEN-END:variables

}
