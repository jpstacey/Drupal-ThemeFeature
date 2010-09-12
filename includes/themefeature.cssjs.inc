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
        $options["theme-$name"] = strtoupper("$type") . " ($media): $name";
      }
    }
  }

  return $options;
}

/**
 * Implementation of componenthook_features_export_render
 */
function cssjs_features_export_render($module_name, $data) {
  module_load_include("inc", "themefeature", "includes/themefeature.files");
  list($theme_name, $theme) = _themefeature_files_determine_theme();
  $files = _themefeature_files_named($theme_name);

  // Match up the data with relevant files where possible
  foreach($files as $type => $bundle) {
    foreach($bundle as $media => $file) {
      foreach($file as $path => $name) {
        foreach($data as $desired_name) {
          if ($desired_name == "theme-$name") {
            $data[$desired_name] = array(
              'type' => $type,
              'media' => $media,
              'path' => $path,
              'name' => $name,
            );
          }
        }
      }
    }
  }

  $code = array();
  $code[] = '$my_objects = array();';
  foreach($data as $k => $v) {
    // Safe key for this component
    $uniq = _themefeature_files_safe_key($module_name . "-" . $v['name']);

    // Ancillary data
    $array = serialize($v);
    $code[] = "\$my_objects['$uniq'] = unserialize('$array');";
    // File data as a HERE document
    $code[] = "\$my_objects['$uniq']['data'] = <<<EOF_CSS_JS";
    $resource = fopen($v['path'], "r");
    $code[] = $resource?fread($resource, filesize($v['path'])):"";
    $resource && fclose($resource);
    $code[] = "EOF_CSS_JS;";
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

