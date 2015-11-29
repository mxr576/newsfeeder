<?php

/**
 * @file
 * Interface and abstract class definitions of the FeedItemEnhancerPlugin.
 */

/**
 * Interface FeedItemEnhancerPluginInterface.
 *
 * Base interface of all Feed Item Enhancer plugins.
 */
interface FeedItemEnhancerPluginInterface {

  /**
   * Construct a FeedItemEnhancerPlugin.
   *
   * @param \FeedsConfigurable $feeds_configurable
   *   Either FeedsImporter or FeedsSource object.
   * @param string $machine_name
   *   Machine name of the plugin.
   * @param array $settings
   *   Settings of the FeedItemEnhancerPlugin.
   */
  public function __construct(FeedsConfigurable $feeds_configurable, $machine_name, array $settings = []);

  /**
   * Display a configuration form for the plugin.
   *
   * @return array
   *   A form array for configuring this plugin, or FALSE if no configuration
   *   is possible.
   */
  public function configurationForm();

  /**
   * Validation callback for the form returned by configurationForm().
   *
   * @param array $form
   *   The form returned by configurationForm().
   * @param array $form_state
   *   The complete form state.
   * @param array $values
   *   The part of the $form_state['values'] array corresponding to this form.
   */
  public function configurationFormValidate(array $form, array &$form_state, array &$values);

  /**
   * Submit callback for the form returned by configurationForm().
   *
   * This method should both return the new options and set them internally.
   *
   * @param array $form
   *   The form returned by configurationForm().
   * @param array $form_state
   *   The complete form state.
   * @param array $values
   *   The part of the $form_state['values'] array corresponding to this form.
   *
   * @return array
   *   The new settings array for this plugins.
   */
  public function configurationFormSubmit(array $form, array &$form_state, array &$values);

  /**
   * Returns the settings of this plugin.
   *
   * @return array
   *   The settings of this plugin.
   */
  public function getSettings();

  /**
   * Process the feed item with this plugin.
   *
   * @param \EntityMetadataWrapper $feed_item_wrapper
   *   Entity meta data wrapper of the feed item.
   * @param array $settings
   *   Settings of the plugin.
   *
   * @return bool
   *   TRUE on success, otherwise FALSE.
   */
  public static function processFeedItem(EntityMetadataWrapper $feed_item_wrapper, array $settings);

}

/**
 * Class FeedItemEnhancerPlugin.
 *
 * Base class of the Feed item enhancer plugins.
 */
abstract class FeedItemEnhancerPlugin implements FeedItemEnhancerPluginInterface {

  /**
   * The Feeds Importer which this Enhancer Plugin belongs to.
   *
   * @var null|FeedsImporter
   */
  protected $feedsImporter;
  /**
   * The Feeds Source which this Enhancer Plugin belongs to.
   *
   * @var null|FeedsSource
   */
  protected $feedsSource;
  /**
   * The machine name of the Enhancer Plugin.
   *
   * @var string
   */
  protected $machineName;
  /**
   * The settings of the Enhancer Plugin.
   *
   * @var array
   */
  protected $settings;

  /**
   * {@inheritdoc}
   */
  public function __construct(FeedsConfigurable $feeds_configurable, $machine_name, array $settings = []) {
    // Set the feedImporter or the feedSource member of the class based on the
    // type of the passed FeedsConfigurable object.
    if ($feeds_configurable instanceof FeedsImporter) {
      $this->feedsImporter = $feeds_configurable;
      $this->feedsSource = NULL;
    }
    elseif ($feeds_configurable instanceof FeedsSource) {
      $this->feedsImporter = feeds_importer($feeds_configurable->__get('id'));
      $this->feedsSource = $feeds_configurable;
    }
    $this->machineName = $machine_name;

    // Make sure that the settings array initialized properly.
    $settings += ['properties' => []];
    $this->settings = $settings;
  }

  /**
   * {@inheritdoc}
   */
  public function getSettings() {
    return $this->settings;
  }

  /**
   * {@inheritdoc}
   */
  public function configurationFormValidate(array $form, array &$form_state, array &$values) {
    // Check all required form element.
    // Form elements can not be marked as required with the #required attribute,
    // because it would cause validation errors, when an enhancer plugin
    // is disabled too.
    foreach ($values as $fieldset => $field) {
      foreach ($field as $name => $value) {
        // If the field marked as required in the #states, then it should
        // not be empty.
        if (isset($form[$fieldset][$name]['#states']['required']) && empty($value)) {
          form_error($form[$fieldset][$name], t('%field field is required!', [
            '%field' => $form[$fieldset][$name]['#title'],
          ]));
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function configurationFormSubmit(array $form, array &$form_state, array &$values) {
    $this->settings = $values;
    return $values;
  }

}
