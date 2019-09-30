<div class="title">Kapcsolat</div>
<br />
<br />
<?php
if ($_GET[n] == 'ok') {
 $comment = '<div class="green_line" id="notice">Az e-mail sikeresen elküldve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'fail') {
 $comment = '<div class="red_line" id="notice">Az e-mail küldése nem sikerült!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'wrong') {
 $comment = '<div class="red_line" id="notice">Valamelyik mezõt nem töltötted ki helyesen!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo $comment;
?>
<form action="?a=main/send&amp;p=main/contact" method="post">
<b>Név:</b><br />
<input type="text" name="name" id="name" onBlur="test('name',5,200)" style="width: 60%" />
<br />
<br />
<b>E-mail cím:</b><br />
<input type="text" name="email" id="email" onBlur="test_mail('email')" style="width: 60%" />
<br />
<br />
<b>Tárgy:</b><br />
<input type="text" name="subject" id="subject" onBlur="test('subject',3,200)" style="width: 60%" />
<br />
<br />
<b>Üzenet:</b><br />
<textarea name="message" id="message" onBlur="test('message',10,10000)" wrap="off" style="width: 80%; height: 200px">
</textarea>
<br />
<br />
<input type="submit" class="button" value="Elküld" />
<input type="reset" class="button" value="Töröl" />
</form>
<br />
