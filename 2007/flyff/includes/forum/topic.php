<?php
if ($_GET[theme]) {
$sql = mysql_query("SELECT * FROM forum WHERE theme_id = '".$_GET[theme]."'");
$entries = mysql_num_rows($sql);
$pages = ceil($entries/10);
$page = $_GET[page];
if ($page == 0) { $page = 1; }
$topic = mysql_fetch_array(mysql_query("SELECT * FROM forum_themes WHERE id = '".$_GET[theme]."'"));
?>
<script type="text/javascript">
function show_reply(id) 
{
  var reply = document.getElementById("reply_"+id) ;
  var link  = document.getElementById("reply_link_"+id) ;

  reply.style.display = "block" ;
  link.style.display  = "none" ;
}
<?php
$edit_teszt = mysql_fetch_array(mysql_query("SELECT id FROM forum WHERE name = '$_SESSION[name]' AND theme_id = '$_GET[theme]' ORDER BY id DESC LIMIT 1"));
$edit_id = $edit_teszt[id];
if ($_SESSION[rank] > 0) {
echo '
function del_entry(id) 
{
  question = window.confirm("Biztosan törölni akarod ezt az üzenetet?") ;
  if(question)
    window.location.href = "?a=forum/del&id=" + id + "&theme='.$_GET[theme].'&page='.$page.'" ;
}
function make_closed() 
{
  question = window.confirm("Biztosan zárolni akarod ezt a fórumtémát?") ;
  if(question)
    window.location.href = "?a=forum/close&theme='.$_GET[theme].'&task=close&page='.$page.'" ;
}
function make_opened() 
{
  question = window.confirm("Biztosan fel akarod oldani a fórumtéma zárolását?") ;
  if(question)
    window.location.href = "?a=forum/close&theme='.$_GET[theme].'&task=open&page='.$page.'" ;
}
';
}
echo '</script>';
if ($_GET[n] == 'success') {
 $comment = '<div class="green_line" id="notice">Az üzenet sikeresen megérkezett!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'edit_ok') {
 $comment = '<div class="green_line" id="notice">Az üzenet módosítása megtörtént!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'edit_fail') {
 $comment = '<div class="red_line" id="notice">Az üzenet módosítása sikertelen volt!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'moved') {
 $comment = '<div class="green_line" id="notice">Az üzenet áthelyezése megtörtént!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'closed') {
 $comment = '<div class="red_line" id="notice">A fórumtéma zárolva van!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'no_rights') {
 $comment = '<div class="red_line" id="notice">Nincs jogod az üzenet módosításához!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'log_in') {
 $comment = '<div class="red_line" id="notice">Üzenet írásához be kell jelentkezni!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_SESSION[rank] > 0) {
 if ($topic[closed] == 1) {
  $openclose = '<b> | <a href="javascript:make_opened()" class="link" title="Fórumtéma feloldása a zárolás alól">Feloldás</a></b>';
 } else {
  $openclose = '<b> | <a href="javascript:make_closed()" class="link" title="Fórumtéma zárolása">Zárolás</a></b>';
 }
}
if ($topic[closed] == 1) {
 $newmessage = '#';
 $closed = '<font class="date">Ez a fórumtéma zárolva van.</font><br />';
} else {
 $newmessage = '?p=forum/write&amp;theme='.$_GET[theme];
}
echo '
<div class="title">'.no_format($topic[theme_name]).'</div>
<br />
<br />
'.$closed.'
'.$comment.'
<b><a href="?p=forum/groups" title="Vissza a fórum fõoldalához.." class="link">Csoportok</a> | <a href="?p=forum/themes&amp;group='.$topic[group_id].'" title="Vissza az aktuális témacsoporthoz.." class="link">Témák</a> | <a href="'.$newmessage.'" class="link">Új hozzászólás</a></b>
'.$openclose.'
<br />
';
$page_minus = $page - 2;
if ($page_minus < 1) { $page_minus = 1; }
$page_plus = $page + 2;
if ($page_plus > $pages) { $page_plus = $pages; }
/* Oldalak számozása */
echo '<div>';
if ($page > 1) {
 echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page=1" class="link"><<</a></div>';
 echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page='.($page-1).'" class="link"><</a></div>';
}
for ($i=$page_minus; $i<=$page_plus; $i++) {
 echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page='.$i.'" class="link">'.$i.'</a></div>';
}
if ($pages > $page) {
 echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page='.($page+1).'" class="link">></a></div>';
 echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page='.$pages.'" class="link">>></a></div>';
}
echo '</div><br /><br />';
$query = mysql_query("SELECT * FROM forum WHERE theme_id = '".$_GET[theme]."' ORDER BY id DESC LIMIT ".(($page-1)*10).", 10");
while ($sql = mysql_fetch_array($query))
{
$topic_entries = mysql_num_rows(mysql_query("SELECT id FROM forum WHERE name = '$sql[name]' AND theme_id = '$_GET[theme]'"));
$forum_entries = mysql_num_rows(mysql_query("SELECT id FROM forum WHERE name = '$sql[name]'"));
$vmi = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '".$sql[name]."' LIMIT 1"));
$id = $sql[id];
$forumname = $vmi[forum_name];
if (strlen($forumname) < 3) { $forumname = $sql[name]; }
$date = $sql[date_text];
$message = $sql[message];
$message = str_replace('  ', ' &nbsp;', $message);
$message = smile(text(images($message)));
$reply = "";
$reply = $sql[reply];
if (strlen($reply) > 0) {
 $reply_link = "<div align=\"right\" id=\"reply_link_".$id."\"><a href=\"javascript:show_reply('".$id."')\" class=\"date\"><img src=\"images/reply_down.gif\" border=\"0\" alt=\"\" />Elõzmény mutatása</a></div>";
} else {
 $reply_link = "";
}
$avatar_img = $vmi[avatar];
if ($avatar_img) {
 $avatar = '<img src="includes/forum/avatar.php?name='.$sql[name].'" class="avatar" alt="" />';
} else {
 $avatar = '<img src="images/no_avatar.jpg" alt="" />';
}
if ($vmi[signature]) {
 $signature = '<br /><br />'.$vmi[signature];
} else {
 $signature = '';
}
if ($_SESSION[name]) {
 $quote = '<div align="right"><a href="?p=forum/write&amp;quote='.$id.'&amp;theme='.$_GET[theme].'" class="date"><img src="images/reply_down.gif" border="0" alt="" />Idézés</a></div>';
} else {
 $quote = '';
}
$torol = '';
if ($id == $edit_id) { $torol = "<a href=\"?p=forum/edit&amp;id=".$id."&amp;theme=".$_GET[theme]."&amp;page=".$page."\" class=\"link\">szerkeszt</a> | "; }
if ($_SESSION[rank] > 0) { $torol = "<a href=\"javascript:del_entry('".$id."')\" class=\"link\">töröl</a>, <a href=\"?p=forum/edit&amp;id=".$id."&amp;theme=".$_GET[theme]."&amp;page=".$page."\" class=\"link\">szerkeszt</a>, <a href=\"?p=forum/move&amp;id=".$id."&amp;theme=".$_GET[theme]."&amp;page=".$page."\" class=\"link\">áthelyez</a> | "; }
echo '
<table class="entry_box" cellspacing="0" cellpadding="0">
<tr>
 <td style="width: 100px"></td>
 <td style="width: 400px"></td>
 <td style="width: 60px"></td>
</tr><tr>
 <td colspan="2" class="entry_title">&nbsp;<a href="?p=main/userdata&amp;user='.$sql[name].'" class="link"><b>'.text($forumname).'</b></a> | '.$torol.$date.' |</td>
 <td class="entry_answer"><a href="?p=forum/write&amp;answer='.$id.'&amp;theme='.$_GET[theme].'" class="link"><b>Válasz</b></a></td>
</tr><tr valign="top">
 <td class="avatar_box">
  '.$avatar.'
 </td>
 <td colspan="2" class="entry_text_box">
  '.$message.'
  '.smile(text($signature)).'
  '.$quote.'
  '.$reply_link.'
 </td>
</tr><tr>
 <td colspan="3" class="entry_info_box">
  <div id="reply_'.$id.'" style="display: none"><b>Elõzmény:</b><br />'.smile(text(images($reply))).'<br /><br /></div>
  <font class="date" style="font-weight: bold">'.no_format($forumname).' üzenetei ebben a témában: '.$topic_entries.', a fórumban: '.$forum_entries.'</font>
 </td>
</tr>
</table>
<br />
';
}
/* Oldalak számozása */
if ($pages > 1) {
 echo '<div>';
 if ($page > 1) {
  echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page=1" class="link"><<</a></div>';
  echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page='.($page-1).'" class="link"><</a></div>';
 }
 for ($i=$page_minus; $i<=$page_plus; $i++) {
  echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page='.$i.'" class="link">'.$i.'</a></div>';
 }
 if ($pages > $page) {
  echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page='.($page+1).'" class="link">></a></div>';
  echo '&nbsp;<div class="page_box"><a href="?p=forum/topic&amp;theme='.$_GET[theme].'&amp;page='.$pages.'" class="link">>></a></div>';
 }
 echo '</div><br /><br />';
}
if ($entries == 0) { echo '<div class="red_line">Még nincs bejegyzés!</div>'; }
}
?>