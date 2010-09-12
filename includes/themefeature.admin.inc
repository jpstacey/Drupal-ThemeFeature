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
  global $theme;
  if (!$theme) { init_theme(); }

  // Work out which theme this is and tell the user
  $themes = list_themes();
  $this_theme = $themes[$theme];
  $name = $this_theme->name ? $this_theme->name : "undefined";
  $form['theme_note']['#value'] = t("Current theme is: '%name'. Examining files in this theme only.", array("%name" => $name));

  // Stylesheets
  $form['stylesheets'] = array(
    '#type' => 'fieldset',
    '#title' => t("Stylesheets"),
  );
  foreach($this_theme->stylesheets as $media => $sheets) {
    $form['stylesheets'][$media] = array(
      '#type' => 'fieldset',
      '#title' => t("Media = @media", array("@media" => $media)),
    );
    foreach($sheets as $name => $path) {
      $form['stylesheets'][$media][$name] = array(
        '#type' => 'textfield',
        '#title' => $name,
        '#maxlength' => 64,
        '#description' => t("Enter an optional feature component name for this file."),
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
  dsm("TODO!");
}
