<?php
if ($_SESSION[rank] > 0) {
 mysql_query("DELETE FROM votes WHERE id = '$_GET[vote_id]' LIMIT 1");
 header('Location: ?p=vote/votes&n=del');
}
?>