<?php
$name = $_POST[name];
$message = $_POST[message];
$mail = $_POST[email];
$subject = $_POST[subject];
if (test($name,5,200) && test($mail,8,50) && test($subject,3,200) && strlen($message) > 10) {
 // $sender_name = $name.' ('.$mail.')'."\n";
 /* Küldés */
 $real_message = 'Feladó: '.$name."\n";
 $real_message .= 'E-mail: '.$mail."\n";
 $real_message .= "\n";
 $real_message .= $message;
 $send = mail('gorzsony@gmail.com', $subject, $real_message, 'From: http://flyff.extra.hu') ;
 if ($send) {
  $re = 'ok';
 } else {
  $re = 'fail';
 }
} else {
 $re = 'wrong';
}
header('Location: ?p=main/contact&n='.$re);
?>