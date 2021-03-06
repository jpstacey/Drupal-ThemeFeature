<?php

/**
 * @file themefeature.cache.inc
 * Cache handling and re-creating in the file_directory_path directory
 */

/**
 * Full cache recreate cycle
 */
function _themefeature_cache_recreate($dir) {
  if (_themefeature_files_directory_exists($dir)) {
    _themefeature_files_clear($dir) && _themefeature_files_create($dir);
  }
}

/**
 * Assert cache directory exists OK; create if not
 */
function _themefeature_files_directory_exists($dir) {
  // Check our cache directory exists
  if (!is_dir($dir)) {
    if (!mkdir($dir)) {
      drupal_set_message("Cannot create temporary directory for theme features.");
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Clear out cache directory
 */
function _themefeature_files_clear($dir) {
  // Delete any existing files
  foreach(array("js", "css") as $suffix) {
    foreach(glob("$dir/*.$suffix") as $file) {
      if(!unlink($file)) {
        drupal_set_message("Cannot delete theme feature file $file.");
        return FALSE;
      }
    }
  }
  return TRUE;
}

/**
 * Create cache files anew
 */
function _themefeature_files_create($dir) {
  module_load_include("inc", "features", "features.export");
  $values = array();
  foreach(features_get_default_map("cssjs") as $component => $module) {
    if (!$values[$module]) {
      features_include_defaults($component);
      $fn = $module . "_cssjs_defaults";
      if (!function_exists($fn)) {
        drupal_set_message("Cannot create theme feature for $component: no supporting function.");
        return FALSE;
      }
      $values[$module] = $fn();
    }
    // Write file with data
    $value = $values[$module][$component];
    $filename = _themefeature_files_safe_key($module . "-" . $value['name']) . "." . $value['type'];
    $file = "$dir/$filename";

    $resource = fopen($file, "w");
    if (!$resource) {
      drupal_set_message("Could not create new theme feature file $file");
      return FALSE;
    }
    fwrite($resource, $value['data']);
    fclose($resource);
  }
  return TRUE;
}

/**
 * Get all named files for a given theme
 */
function _themefeature_files_named($named_theme = NULL) {
  if (!isset($named_theme)) {
    global $theme;
    init_theme();
    $named_theme = $theme;
  }

  $files = array();
  $result = db_query("SELECT * FROM {themefeature_name} WHERE theme = '%s' ORDER BY type, media, file", $named_theme);
  while($row = db_fetch_array($result)) {
    $files[$row['type']][$row['media']][$row['file']] = $row['name'];
  }
  return $files;
}

/**
 * Safe filename
 */
function _themefeature_files_safe_key($text) {
  return preg_replace("/-+/", "-", preg_replace("/\W/", "-", strtolower($text)));
}

/**
 * Work out and return default theme
 */
function _themefeature_files_determine_theme() {
  global $theme;
  if (!$theme) { init_theme(); }

  // Work out which theme this is and tell the user
  $themes = list_themes();
  $this_theme = $themes[$theme];

  return array($theme, $this_theme);
}
