<?php $vmi = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$_SESSION[name]'")); ?>
<div class="title">Beállítások</div>
<br />
<br />
<?php
if ($_GET[n] == 'pwchanged') {
 $comment = '<div class="green_line" id="notice">A jelszó sikeresen megváltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw') {
 $comment = '<div class="red_line" id="notice">Az egyik jelszó nem megfelelõ!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw2') {
 $comment = '<div class="red_line" id="notice">A két megadott jelszó nem egyezik!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badpw3') {
 $comment = '<div class="red_line" id="notice">A régi jelszó nem egyezik!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'char_ok') {
 $comment = '<div class="green_line" id="notice">A karakterek sikeresen megváltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'char_fail') {
 $comment = '<div class="red_line" id="notice">A mezõ nem volt megfelelõen kitöltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'fullname_ok') {
 $comment = '<div class="green_line" id="notice">A teljes név sikeresen megváltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'fullname_fail') {
 $comment = '<div class="red_line" id="notice">A mezõ nem volt megfelelõen kitöltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'mail_ok') {
 $comment = '<div class="green_line" id="notice">Az e-mail cím sikeresen megváltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'mail_fail') {
 $comment = '<div class="red_line" id="notice">A megadott e-mail cím nem megfelelõ!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'forumname_ok') {
 $comment = '<div class="green_line" id="notice">A fórum név sikeresen megváltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'forumname_fail') {
 $comment = '<div class="red_line" id="notice">A mezõ nem volt megfelelõen kitöltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'avatar_ok') {
 $comment = '<div class="green_line" id="notice">A fórum avatár sikeresen feltöltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'badavatar') {
 $comment = '<div class="red_line" id="notice">A feltöltött avatár típusa nem megfelelõ!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'bigavatar') {
 $comment = '<div class="red_line" id="notice">A feltöltött avatár mérete meghaladja az 500KB-ot!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'noavatar') {
 $comment = '<div class="red_line" id="notice">Sikertelen avatár feltöltési kísérlet!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'delavatar') {
 $comment = '<div class="green_line" id="notice">A fórum avatár sikeresen törölve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'sign_ok') {
 $comment = '<div class="green_line" id="notice">A fórum aláírás sikeresen megváltoztatva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'sign_fail') {
 $comment = '<div class="red_line" id="notice">A megadott aláírás túl hosszú!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo $comment;
?>
<table width="100%"><tr valign="top">
<td width="50%">
<form action="?a=main/setprofil&amp;save=1&amp;p=main/options" method="post">
<b>Régi jelszó:</b><br /><input type="password" name="pass_regi" style="width: 90%" />
<br /><br />
<b>Jelszó:</b><br /><input type="password" name="pass_elso" id="pass_elso" onBlur="test('pass_elso',3,20)" style="width: 90%" />
<br /><br />
<b>Jelszó mégegyszer:</b><br /><input type="password" name="pass_masodik" id="pass_masodik" onBlur="test_pw('pass_masodik','pass_elso')" style="width: 90%" />
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
<b>Teljes név:</b><br /><?php echo "<input type=\"text\" name=\"fullname\" id=\"fullname\" onBlur=\"test('fullname',0,50)\" value=\"$vmi[full_name]\" style=\"width: 90%\" />"; ?>
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
<br /><br />
<form action="?a=main/setprofil&amp;save=4&amp;p=main/options" method="post">
<b>E-mail cím:</b><br /><?php echo "<input type=\"text\" name=\"email\" id=\"email\" onBlur=\"test_mail('email')\" value=\"$vmi[email]\" style=\"width: 90%\" />"; ?>
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
<br /><br />
<form action="?a=main/setprofil&amp;save=5&amp;p=main/options" method="post">
<b>Fórumban látszódó név:</b><br /><?php echo "<input type=\"text\" name=\"forum_name\" id=\"forum_name\" onBlur=\"test('forum_name',0,50)\" value=\"$vmi[forum_name]\" style=\"width: 90%\" />"; ?>
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
<br /><br />
<form enctype="multipart/form-data" action="?a=main/setprofil&amp;save=6&amp;p=main/options" method="POST">
<b>Fórum avatár:</b><br />
<input name="userfile" type="file" />
<br /><br />
<input type="submit" value="Feltöltés" class="button" />
<input type="button" onClick="document.location = '?a=main/setprofil&amp;save=6&amp;e=del&amp;p=main/options'" value="Töröl" class="button" />
</form>
<br /><br />
<form action="?a=main/setprofil&amp;save=7&amp;p=main/options" method="post">
<b>Fórum aláírás:</b><br /><?php echo "<input type=\"text\" name=\"signature\" id=\"signature\" onBlur=\"test('signature',0,300)\" value=\"$vmi[signature]\" style=\"width: 90%\" />"; ?>
<br /><br />
<input type="submit" class="button" value="Rendben" />
</form>
</td>
</tr></table><br />