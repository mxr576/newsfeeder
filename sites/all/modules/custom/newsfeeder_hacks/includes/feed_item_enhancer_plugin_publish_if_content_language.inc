<?php
/**
 * @file
 * Contains the definition of the Publish If Content Language FE. plugin.
 */

/**
 * Class PublishIfContentLanguageFeedItemEnhancerPlugin.
 *
 * Retrieve the full content of the feed item from the feed item's source
 * link.
 */
class PublishIfContentLanguageFeedItemEnhancerPlugin extends NodeFeedItemEnhancerPlugin {
  /**
   * {@inheritdoc}
   */
  public function configurationForm() {
    include_once DRUPAL_ROOT . '/includes/iso.inc';
    $languages = _locale_get_predefined_list();
    $options = array_map(function ($language) {
      return $language[0];
    }, $languages);

    unset($options['en-gb']);
    $options['en'] = t('English');

    $form['publish_if']['language'] = [
      '#type' => 'select',
      '#title' => t('Language'),
      '#description' => t("Publish a feed item, if the feed item's language is the selected one."),
      '#default_value' => isset($this->settings['publish_if']['language']) ? $this->settings['publish_if']['language'] : 'en',
      '#empty_option' => t('- Select -'),
      '#options' => $options,
      '#states' => [
        'required' => ['input[name="enhancers[' . $this->machineName . '][status]"]' => ['checked' => TRUE]],
      ],
    ];

    return array_merge($form, parent::configurationForm());
  }

  /**
   * {@inheritdoc}
   */
  public static function processFeedItem(EntityMetadataWrapper $feed_item_wrapper, array $settings) {
    if ($feed_item_wrapper->language->value() == $settings['publish_if']['language']) {
      $feed_item_wrapper->status = NODE_PUBLISHED;
      return $feed_item_wrapper->save();
    }

    return TRUE;
  }

}
