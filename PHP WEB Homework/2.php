<html>
<title> [ PHP & Raspberry Pi ] </title>
<script language='javascript'>
window.setTimeout('window.location.reload()', 1000);
</script>
<body>
<h3>Displaying an Image</h3>
<?php 
    if(file_exists('image.jpeg')){
        $imagePath="image.jpeg";
    }
    else{
        $imagePath = "default.jpg";
    }
?>
<img src=<?php echo $imagePath; ?> alt="Sample Image" style="max-width:100%; height:auto;">
</body>
</html>