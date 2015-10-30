<?php
/**
 * @file
 * Contains administration pages of the NewsFeeder Hacks.
 */

/**
 * Page callback for admin/structure/feeds/newsfeeder-hacks.
 */
function newsfeeder_hacks_config_form($form, &$form_state) {
  $form['delete-unpublished-feed-items'] = [
    '#type' => 'fieldset',
    '#title' => t('Delete unpublished feed items'),
    '#description' => t('Delete unpublished feed (node) items of the selected feed importers in the given period automatically.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ];

  $form['delete-unpublished-feed-items'][NEWSFEEDER_HACKS_UNPUBLISHED_DELETE_FEED_IMPORTERS] = [
    '#type' => 'select',
    '#required' => FALSE,
    '#title' => t('Feeds importers'),
    '#multiple' => TRUE,
    '#description' => t("Select which feeds importers' unpublished feed (node) items should be deleted periodically."),
    '#default_value' => variable_get(NEWSFEEDER_HACKS_UNPUBLISHED_DELETE_FEED_IMPORTERS, ''),
    '#empty_option' => t('- Select -'),
    '#options' => _newsfeeder_hacks_config_form_feed_importers_with_node_processor_options(),
  ];

  $form['delete-unpublished-feed-items'][NEWSFEEDER_HACKS_UNPUBLISHED_DELETE_PERIOD] = [
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Period'),
    '#default_value' => variable_get(NEWSFEEDER_HACKS_UNPUBLISHED_DELETE_PERIOD, NEWSFEEDER_HACKS_UNPUBLISHED_DELETE_DEFAULT_PERIOD),
    '#empty_option' => t('- Select -'),
    '#options' => _newsfeeder_hacks_config_form_period_options(),
  ];

  return system_settings_form($form);
}

/**
 * Option callback for Feeds importers form element.
 *
 * @see newsfeeder_hacks_config_form()
 */
function _newsfeeder_hacks_config_form_feed_importers_with_node_processor_options() {
  $node_importers = newsfeeder_hacks_get_feed_importers_with_node_processors();
  return array_map(function ($importer) {
    $config = $importer->getConfig();
    return $config['name'];
  }, $node_importers);
}

/**
 * Option callback for Period form element.
 *
 * @see newsfeeder_hacks_config_form()
 */
function _newsfeeder_hacks_config_form_period_options() {
  return [
    '4 0 * * *' => t('Every days'),
    '4 0 * * 0' => t('Weekly'),
    '4 0 1 * *' => t('Monthly'),
  ];
}
