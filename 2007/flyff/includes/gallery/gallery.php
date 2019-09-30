<script type="text/javascript" language="JavaScript">
function upload(id) 
{
  var upload      = document.getElementById("upload") ;
  var upload_link = document.getElementById("upload_link") ;

  upload.style.display      = "block" ;
  upload_link.style.display = "none" ;
}
function del_img(id) 
{
  question = window.confirm("Biztosan törölni akarod ezt a képet?") ;
  if(question)
    window.location.href = "?a=gallery/del&id=" + id + <?php echo '"&page='.$_GET[page].'"'; ?> ;
}
</script>
<div class="title">Galéria</div>
<br />
<br />
<?php
if ($_GET[n] == 'success') {
 $comment = '<div class="green_line" id="notice">A kép sikeresen feltöltve!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'del') {
 $comment = '<div class="red_line" id="notice">A kép sikeresen eltávolítva!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'full') {
 $comment = '<div class="red_line" id="notice">Ezzel a felhasználónévvel nem tölthetsz fel több képet!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'badfile') {
 $comment = '<div class="red_line" id="notice">A feltöltött fájl típusa nem megfelelõ!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'bigfile') {
 $comment = '<div class="red_line" id="notice">A feltöltött fájl mérete meghaladja az 500KB-ot!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
if ($_GET[n] == 'nofile') {
 $comment = '<div class="red_line" id="notice">Sikertelen feltöltési kísérlet!</div><script language="JavaScript" type="text/javascript">window.setTimeout("HideNoticeMessage(0.9)", 3500);</script>';
}
echo $comment;
$page = $_GET[page];
if ($page == 0) { $page = 1; }
if ($_GET[user]) {
 if ($_SESSION[rank] == 0) {
  $hiddens = "hidden = 0 AND";
 }
 $user = '&user='.$_GET[user];
 $kepek = mysql_num_rows(mysql_query("SELECT id FROM images WHERE $hiddens name = '$_GET[user]'"));
 $query = mysql_query("SELECT * FROM images WHERE $hiddens name = '$_GET[user]' ORDER BY id DESC LIMIT ".(($page-1)*12).", 12");
} else {
 if ($_SESSION[rank] == 0) {
  $hiddens = "WHERE hidden = 0";
 }
 $kepek = mysql_num_rows(mysql_query("SELECT id FROM images $hiddens"));
 $query = mysql_query("SELECT * FROM images $hiddens ORDER BY id DESC LIMIT ".(($page-1)*12).", 12");
}
if ($kepek > 0) {
$pages = ceil($kepek/12);
$page_minus = $page - 2;
if ($page_minus < 1) { $page_minus = 1; }
$page_plus = $page + 2;
if ($page_plus > $pages) { $page_plus = $pages; }
/* Oldalak számozása */
echo '<div>';
if ($page > 1) {
 echo '&nbsp;<div class="page_box"><a href="?p=gallery/gallery'.$user.'&amp;page=1" class="link"><<</a></div>';
 echo '&nbsp;<div class="page_box"><a href="?p=gallery/gallery'.$user.'&amp;page='.($page-1).'" class="link"><</a></div>';
}
for ($i=$page_minus; $i<=$page_plus; $i++) {
 echo '&nbsp;<div class="page_box"><a href="?p=gallery/gallery'.$user.'&amp;page='.$i.'" class="link">'.$i.'</a></div>';
}
if ($pages > $page) {
 echo '&nbsp;<div class="page_box"><a href="?p=gallery/gallery'.$user.'&amp;page='.($page+1).'" class="link">></a></div>';
 echo '&nbsp;<div class="page_box"><a href="?p=gallery/gallery'.$user.'&amp;page='.$pages.'" class="link">>></a></div>';
}
echo '</div><br /><br />'."\n";
echo '<table cellspacing="10" cellpadding="0"><tr>'."\n";
// Képek listázása
$i = 0;
while ($kepek_sql = mysql_fetch_array($query)) {
 $i = $i + 1;
 if ($i == 5) {
  $i = 1;
  echo "</tr><tr>\n";
 }
 if ($kepek_sql[hidden] == 0) {
  $hidden = '';
 } else {
  $hidden = '_dotted';
 }
 echo '<td class="thumb'.$hidden.'" align="center"><br />'."\n";
 $sql = mysql_fetch_array(mysql_query("SELECT name,forum_name,rank FROM users WHERE name = '$kepek_sql[name]'"));
 $forumname = $sql[forum_name];
 if (strlen($forumname) < 3) { $forumname = $sql[name]; }
 if ($_SESSION[rank] > $sql[rank] or $_SESSION[name] == $sql[name]) {
  $del = "<a href=\"javascript:del_img('$kepek_sql[id]')\" title=\"Kép törlése\"><img src=\"images/delete.png\" border=\"0\" alt=\"\" /></a>";
 }
 echo '
 <a href="?a=gallery/image&amp;id='.$kepek_sql[id].'" title="Megnyitás nagy méretben" target="_blank">
 <img src="images/gallery/'.$kepek_sql[id].'_th.jpg" border="0" alt="" />
 </a>
 <br />
 <b><a href="?p=main/userdata&amp;user='.$sql[name].'" class="link">'.text($forumname).'</a></b>
 <a href="?p=gallery/gallery&amp;user='.$sql[name].'" title="Képek ettõl a felhasználótól"><img src="images/reply_down.gif" border="0" alt="" /></a>
 <br />
 '.$kepek_sql[date].'<br />'.$del.'
 </td>';
}
if ($i < 4) {
 for ($k=0; $k<(4-$i); $k++) {
  echo '<td></td>';
 }
}
echo '</tr></table>';
} else {
 echo '<div class="red_line">Még nincs kép!</div>';
}
if ($_SESSION[name]) {
 if ($_SESSION[rank] > 0) { 
  $hidden = '<br /><input type="checkbox" name="hidden" /> A kép legyen rejtett.';
 }
?>
<br />
<br />
<div id="upload_link">
<a href="javascript:upload()" class="link"><img src="images/reply_down.gif" border="0" alt="" /><b> Új kép feltöltése</b></a><br />
</div>
<div id="upload" style="display: none">
<form enctype="multipart/form-data" action="?a=gallery/upload&amp;p=gallery/gallery" method="POST">
Kérlek válassz egy képet, majd kattints a "Feltöltés" gombra. A megadott kép nem lehet nagyobb mint 500KB és JPG típusúnak kell lennie.
<br /><br />
<input name="userfile" type="file" /><?php echo $hidden; ?>
<br /><br />
<input type="submit" value="Feltöltés" class="button" />
</form>
</div><br />
<?php
} else {
 echo '<br />';
}
?>