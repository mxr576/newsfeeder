<?php
/**
 * @file
 * NewsFeeder Daily Search Results administration pages.
 */

/**
 * Administration page for NewsFeeder Daily Search Results.
 */
function newsfeeder_daily_search_results_settings_form($form, &$form_state) {
  $form[NEWSFEEDER_DAILY_SEARCH_RESULTS_GENERATED_FILE_NAME_PREFIX] = [
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('File name prefix'),
    '#description' => t('Prefix of the generated csv file.'),
    '#default_value' => variable_get(NEWSFEEDER_DAILY_SEARCH_RESULTS_GENERATED_FILE_NAME_PREFIX, 'nf_daily_search_results'),
  ];

  return system_settings_form($form);
}

/**
 * Validation handler.
 *
 * @see newsfeeder_daily_search_results_settings_form()
 */
function newsfeeder_daily_search_results_settings_form_validate($form, &$form_state) {
  // Transliterate the provided filename.
  $form_state['values'][NEWSFEEDER_DAILY_SEARCH_RESULTS_GENERATED_FILE_NAME_PREFIX] = transliteration_clean_filename($form_state['values'][NEWSFEEDER_DAILY_SEARCH_RESULTS_GENERATED_FILE_NAME_PREFIX]);
}
