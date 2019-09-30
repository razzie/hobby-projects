<?php
if ($_SESSION[rank] > 0) {
echo '<script type="text/javascript">
function news_del(id) 
{
  question = window.confirm("Biztosan törölni akarod ezt a hírt?") ;
  if(question)
    window.location.href = "?a=main/news_del&id=" + id ;
}
</script>';
}
if ($_GET[n] == 'ok') {
 $adatok = mysql_fetch_array(mysql_query("SELECT name, forum_name FROM users WHERE name = '$_SESSION[name]'"));
 $forumname = $adatok[forum_name];
 if (strlen($forumname) < 3) { $forumname = $_SESSION[name]; }
 $comment = '<div class="green_line" id="notice">Sikeresen bejelentkeztél, '.text($forumname).'!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[success] == '1') {
 $comment = '<div class="green_line" id="notice">A hír sikeresen megérkezett!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[success] == '0') {
 $comment = '<div class="red_line" id="notice">Helytelenül töltötted ki az egyik mezõt!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo $comment;
$java = "'";
$id = $_GET[news_id];
if ($id) {
 $where = "WHERE id > '".($id-3)."' AND id < '".($id+3)."'";
}
$sql = mysql_query("SELECT * FROM news $where ORDER BY id DESC LIMIT 5");
while ($vmi = mysql_fetch_array($sql)) {
$adatok = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '".$vmi[owner]."' LIMIT 1"));
$news = $vmi[text];
$news = str_replace('  ', ' &nbsp;', $news);
$news = smile(text(images($news)));
$title = text($vmi[title]);
$forumname = $adatok[forum_name];
if (strlen($forumname) < 3) { $forumname = $vmi[owner]; }
if ($_SESSION[rank] > 0) { $torol = '<a href="javascript:news_del('.$java.$vmi[id].$java.')" class="link">töröl</a>,
 <a href="?p=main/news_edit&amp;id='.$vmi[id].'" class="link">szerkeszt</a> | '; }
if ($id && $vmi[id] == $id) {
 $entry .= '<a name="jump"></a>';
}
$entry .= '<div class="news_title">
<div style="float: left">&nbsp;'.$title.'<br /></div>
<div style="float: right; text-align: right" class="date">
 | '.$torol.'<a href="?p=main/userdata&amp;user='.$vmi[owner].'" class="link">'.text($forumname).'</a> |&nbsp;<br />
 | '.$vmi[date_text].' |&nbsp;
</div>
</div><br />
'.$news.'
<br /><br />
<hr class="dashed_line" />';
}
if ($entry == "") {
$entry = '
<div class="title">Nyitóoldal / Hírek</div>
<br />
<br />
<div class="red_line">Még nincs tartalom!</div>
';
}
echo $entry;
if ($_SESSION[rank] > 0) {
 echo '<br /><center>[ <a href="?p=main/news_write" title="Új hír beküldése" class="link">Új hír..</a> ]</center><br /><br />';
}
?>
