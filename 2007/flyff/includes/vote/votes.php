<?php
if ($_SESSION[rank] > 0) {
echo '<script type="text/javascript">
function vote_del(id) 
{
  question = window.confirm("Biztosan t�r�lni akarod ezt a szavaz�st?") ;
  if(question)
    window.location.href = "?a=vote/del&vote_id=" + id ;
}
</script>';
}
if ($_GET[n] == 'new') {
 $comment = '<div class="green_line" id="notice">A szavaz�s sikeresen l�trehozva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'edit') {
 $comment = '<div class="green_line" id="notice">A szavaz�s sikeresen szerkesztve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'voted') {
 $comment = '<div class="green_line" id="notice">Sikeresen szavazt�l!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'cantvote') {
 $comment = '<div class="red_line" id="notice">Te m�r szavazt�l!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
if ($_GET[n] == 'del') {
 $comment = '<div class="red_line" id="notice">A szavaz�s t�r�lve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script><br />';
}
echo '
<script type="text/javascript" language="JavaScript">
function all_votes() {
 var votes      = document.getElementById("all_votes");
 var votes_link = document.getElementById("all_votes_link");
 votes.style.display      = "block";
 votes_link.style.display = "none";
}
</script>
<div class="title">Szavaz�sok</div>
<br />
<br />
'.$comment.'
<table width="100%"><tr valign="top">
<td width="50%">
';
if ($_SESSION[rank] > 0) {
 echo '<b><a href="?p=vote/new" class="link">�j szavaz�s</a></b><br /><br />';
}
$query = mysql_query("SELECT * FROM votes ORDER BY id DESC");
$votes = mysql_num_rows($query);
$i = 0;
while ($sql = mysql_fetch_array($query)) {
 $i = $i + 1;
 if ($i == 11) { echo '<div id="all_votes" style="display: none">'; }
 echo '<img src="images/arrow.png" alt="" />&nbsp<a href="?p=vote/votes&amp;id='.$sql[id].'">'.$sql[title].'</a><br />'."\n";
}
if ($i >= 11) {
 echo '</div><div id="all_votes_link"><br /><a href="javascript:all_votes()" class="date"><img src="images/reply_down.gif" border="0" alt="" /> �sszes szavaz�s..</a></div>';
}
echo '</td><td width="50%">';
$id = $_GET[id];
$query = mysql_query("SELECT * FROM votes WHERE id = '$id' LIMIT 1");
$sql = mysql_fetch_array($query);
if ($votes && $id) {
 echo '<div class="menu_title">Szavaz�s</div><div class="menu_box">';
 $count = 0;
 $owner_sql = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$sql[owner]' LIMIT 1"));
 $owner = $owner_sql[forum_name];
 if (strlen($owner) < 3) { $owner = $sql[owner]; }
 echo '<b>'.text($sql[title]).'</b><br /><br />';
 if (substr_count($sql[people], '*'.$_SESSION[name].'*') == 0 && $_SESSION[name]) {
  for ($i=1; $i<=8; $i++)
  {
   if ($sql['answer'.$i]) {
    echo '<a href="?a=vote/voting&amp;id='.$sql[id].'&amp;answer='.$i.'&amp;p=vote/votes">'.text($sql['answer'.$i]).'</a><hr class="dashed_line" />';
   }
  }
 } else {
  for ($i=1; $i<=8; $i++)
  {
   if ($sql['answer'.$i]) { $answers += 1; }
   $count += $sql['count'.$i];
  }
  for ($i=1; $i<=8; $i++)
  {
   if ($sql['answer'.$i]) {
    if ($sql['count'.$i] == 0) {
     $length = 0;
     $percent = 0;
    } else {
     $length = ceil($sql['count'.$i]/$count*100);
     $percent = round($sql['count'.$i]/$count*100, 1);
    }
    echo '&nbsp;'.text($sql['answer'.$i]).'<br />'."\n";
    echo '<div style="width: 100%">'."\n";
    echo '<div class="vote"><div class="vote_green" style="width: '.$length.'px"></div></div>'."\n";
    echo '<div style="float: right">&nbsp;<a title="'.$sql['count'.$i].' szavazat" class="percent"> '.$percent.'%</a></div><br />'."\n";
    echo '</div>'."\n";
   }
  }
 }
 if ($_SESSION[rank] > 0) {
  $torol = "<br />( <a href=\"javascript:vote_del('".$id."')\" class=\"date\">t�rl�s</a>, <a href=\"?p=vote/edit&amp;id=".$id."\" class=\"date\">szerkeszt�s</a> )";
 }
 echo '<br /><a href="?p=main/userdata&amp;user='.$sql[owner].'" class="link">'.text($owner).'</a>'.$torol.'</div>';
}
?>
</td>
</tr></table><br />