<?php $vmi = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$_SESSION[name]'")); ?>
<div class="title">Be�ll�t�sok</div>
<br />
<br />
<?php
if ($_GET[n] == 'pwchanged') {
 $comment = '<div class="green_line" id="notice">A jelsz� sikeresen megv�ltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw') {
 $comment = '<div class="red_line" id="notice">Az egyik jelsz� nem megfelel�!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw2') {
 $comment = '<div class="red_line" id="notice">A k�t megadott jelsz� nem egyezik!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw3') {
 $comment = '<div class="red_line" id="notice">A r�gi jelsz� nem egyezik!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'char_ok') {
 $comment = '<div class="green_line" id="notice">A karakterek sikeresen megv�ltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'char_fail') {
 $comment = '<div class="red_line" id="notice">A mez� nem volt megfelel�en kit�ltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'fullname_ok') {
 $comment = '<div class="green_line" id="notice">A teljes n�v sikeresen megv�ltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'fullname_fail') {
 $comment = '<div class="red_line" id="notice">A mez� nem volt megfelel�en kit�ltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'mail_ok') {
 $comment = '<div class="green_line" id="notice">Az e-mail c�m sikeresen megv�ltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'mail_fail') {
 $comment = '<div class="red_line" id="notice">A megadott e-mail c�m nem megfelel�!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'forumname_ok') {
 $comment = '<div class="green_line" id="notice">A f�rum n�v sikeresen megv�ltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'forumname_fail') {
 $comment = '<div class="red_line" id="notice">A mez� nem volt megfelel�en kit�ltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'avatar_ok') {
 $comment = '<div class="green_line" id="notice">A f�rum avat�r sikeresen felt�ltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badavatar') {
 $comment = '<div class="red_line" id="notice">A felt�lt�tt avat�r t�pusa nem megfelel�!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'bigavatar') {
 $comment = '<div class="red_line" id="notice">A felt�lt�tt avat�r m�rete meghaladja az 500KB-ot!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'noavatar') {
 $comment = '<div class="red_line" id="notice">Sikertelen avat�r felt�lt�si k�s�rlet!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'delavatar') {
 $comment = '<div class="green_line" id="notice">A f�rum avat�r sikeresen t�r�lve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'sign_ok') {
 $comment = '<div class="green_line" id="notice">A f�rum al��r�s sikeresen megv�ltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'sign_fail') {
 $comment = '<div class="red_line" id="notice">A megadott al��r�s t�l hossz�!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo $comment;
?>
<table width="100%"><tr valign="top">
<td width="50%">
<form action="?a=main/setprofil&amp;save=1&amp;p=main/options" method="post">
<b>R�gi jelsz�:</b><br /><input type="password" name="pass_regi" style="width: 90%" />
<br /><br />
<b>Jelsz�:</b><br /><input type="password" name="pass_elso" id="pass_elso" onBlur="test('pass_elso',3,20)" style="width: 90%" />
<br /><br />
<b>Jelsz� m�gegyszer:</b><br /><input type="password" name="pass_masodik" id="pass_masodik" onBlur="test_pw('pass_masodik','pass_elso')" style="width: 90%" />
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
<br /><br />
<form action="?a=main/setprofil&amp;save=2&amp;p=main/options" method="post">
<b>Karakter(ek):</b><br /><textarea name="characters" id="characters" onBlur="test('characters',0,2000)" style="width: 90%; height: 160px"><?php echo $vmi[characters]; ?></textarea>
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
</td><td width="50%">
<form action="?a=main/setprofil&amp;save=3&amp;p=main/options" method="post">
<b>Teljes n�v:</b><br /><?php echo "<input type=\"text\" name=\"fullname\" id=\"fullname\" onBlur=\"test('fullname',0,50)\" value=\"$vmi[full_name]\" style=\"width: 90%\" />"; ?>
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
<br /><br />
<form action="?a=main/setprofil&amp;save=4&amp;p=main/options" method="post">
<b>E-mail c�m:</b><br /><?php echo "<input type=\"text\" name=\"email\" id=\"email\" onBlur=\"test_mail('email')\" value=\"$vmi[email]\" style=\"width: 90%\" />"; ?>
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
<br /><br />
<form action="?a=main/setprofil&amp;save=5&amp;p=main/options" method="post">
<b>F�rumban l�tsz�d� n�v:</b><br /><?php echo "<input type=\"text\" name=\"forum_name\" id=\"forum_name\" onBlur=\"test('forum_name',0,50)\" value=\"$vmi[forum_name]\" style=\"width: 90%\" />"; ?>
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
<br /><br />
<form enctype="multipart/form-data" action="?a=main/setprofil&amp;save=6&amp;p=main/options" method="POST">
<b>F�rum avat�r:</b><br />
<input name="userfile" type="file" />
<br /><br />
<input type="submit" value="Felt�lt�s" class="button" />
<input type="button" onClick="document.location = '?a=main/setprofil&amp;save=6&amp;e=del&amp;p=main/options'" value="T�r�l" class="button" />
</form>
<br /><br />
<form action="?a=main/setprofil&amp;save=7&amp;p=main/options" method="post">
<b>F�rum al��r�s:</b><br /><?php echo "<input type=\"text\" name=\"signature\" id=\"signature\" onBlur=\"test('signature',0,300)\" value=\"$vmi[signature]\" style=\"width: 90%\" />"; ?>
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
</td>
</tr></table><br />