<?php

/**
 * @file themefeature.admin.inc
 * Administer theme features
 */

/**
 * Form API callback: preparing exportable features from theme files
 */
function themefeature_admin_prepare(&$form_state) {
  $form['#tree'] = TRUE;

  // Ensure we have a theme to work with
  module_load_include("inc", "themefeature", "includes/themefeature.files");
  list($theme, $this_theme) = _themefeature_files_determine_theme();

  $name = $this_theme->name ? $this_theme->name : "undefined";
  $form['theme_note']['#value'] = t("Current theme is: '%name'. Examining files in this theme only.", array("%name" => $name));

  // Existing named files
  module_load_include("inc", "themefeature", "includes/themefeature.files");
  $files = _themefeature_files_named($theme);

  // Stylesheets
  $form['css'] = array(
    '#type' => 'fieldset',
    '#title' => t("Stylesheets"),
  );
  foreach($this_theme->stylesheets as $media => $sheets) {
    $form['css'][$media] = array(
      '#type' => 'fieldset',
      '#title' => t("Media = @media", array("@media" => $media)),
    );
    foreach($sheets as $name => $path) {
      $form['css'][$media][$path] = array(
        '#type' => 'textfield',
        '#title' => $path,
        '#maxlength' => 64,
        '#description' => t("Enter an optional feature component name for this file."),
        '#default_value' => $files['css'][$media][$path],
      );
    }
  }
  // Javascript
  if ($this_theme->scripts) {
    $form['js'] = array(
      '#type' => 'fieldset',
      '#title' => t("Javascripts"),
    );
    foreach($this_theme->scripts as $name => $path) {
      $form['js'][$path] = array(
        '#type' => 'textfield',
        '#title' => $path,
        '#maxlength' => 64,
        '#description' => t("Enter an optional feature component name for this file."),
        '#default_value' => $files['js']['default'][$path],
      );
    }
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form API submit callback: save names
 */
function themefeature_admin_prepare_submit(&$form, &$form_state) {
  global $theme;

  db_query("TRUNCATE themefeature_name");
  foreach($form_state['values']['css'] as $media => $sheets) {
    foreach($sheets as $file => $name) {
      if (!trim($name)) { continue; }
      $record = array(
        'theme' => $theme,
        'file' => $file,
        'type' => 'css',
        'media' => $media,
        'name' => $name,
      );
      drupal_write_record("themefeature_name", $record);
    }
  }
  foreach($form_state['values']['js'] as $file => $name) {
    if (!trim($name)) { continue; }
    $record = array(
      'theme' => $theme,
      'file' => $file,
      'type' => 'js',
      'media' => 'default',
      'name' => $name,
    );
    drupal_write_record("themefeature_name", $record);
  }
}
