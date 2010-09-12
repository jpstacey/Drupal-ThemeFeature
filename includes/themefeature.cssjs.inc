<?php

/**
 * @file themefeature.cssjs.inc
 * Feature component administration for CSS/JS
 */

/**
 * Implementation of componenthook_features_export
 */
function cssjs_features_export($data, &$export, $module_name) {
  // The following is the simplest implementation of a straight object export
  // with no further export processors called.

  foreach ($data as $component) {
    $export['features']['cssjs'][$component] = $component;
  }
  return array();
}

/**
 * Implementation of componenthook_features_export_options
 */
function cssjs_features_export_options() {
  // Any options provided by enabled features
  foreach(features_get_default_map("cssjs") as $component => $module) {
    $options[$component] = $component;
  }

  // Extra options
  module_load_include("inc", "themefeature", "includes/themefeature.files");
  $named_files = _themefeature_files_named();
  foreach($named_files as $type => $bundle) {
    foreach($bundle as $media => $files) {
      foreach($files as $file => $name) {
        $options["theme-$type-$media-$file"] = strtoupper("$type") . " ($media): $name";
      }
    }
  }

  return $options;
}

/**
 * Implementation of componenthook_features_export_render
 */
function cssjs_features_export_render($module_name, $data) {
  $code = array();
  $code[] = '$my_objects = array();';
  foreach($data as $k => $v) {
    $code[] = "\$my_objects['$v'] = <<<EOF";
    $code[] = "h1,h2,h3,h4,h5,h6 { color: red !important; }";
    $code[] = "EOF;";
  }
  $code[] = "return \$my_objects;";
  $code = implode("\n", $code);

  return array('cssjs_defaults' => $code);
}

/**
 * Implementation of componenthook_features_export_revert
 */
function cssjs_features_export_revert($module_name) {
  $mycomponents = module_invoke_all($module_name, 'cssjs_defaults');
  if (!empty($mycomponents)) {
    foreach ($mycomponents as $mycomponent) {
      watchdog("cssjs", "revert $mycomponent");
    }
  }
  return TRUE;
}

/**
 * Implementation of componenthook_features_export_rebuild
 */
function cssjs_features_export_rebuild($module_name) {
  $mycomponents = module_invoke_all($module_name, 'cssjs_defaults');
  if (!empty($mycomponents)) {
    foreach ($mycomponents as $mycomponent) {
      watchdog("cssjs", "build $mycomponent");
    }
  }
}

