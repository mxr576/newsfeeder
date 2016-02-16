<?php
/**
 * @file
 * newsfeeder_demo_cts.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function newsfeeder_demo_cts_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-feed_item-field_feed_item_authors'.
  $field_instances['node-feed_item-field_feed_item_authors'] = array(
    'bundle' => 'feed_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_feed_item_authors',
    'label' => 'Author(s)',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-feed_item-field_feed_item_categories'.
  $field_instances['node-feed_item-field_feed_item_categories'] = array(
    'bundle' => 'feed_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_feed_item_categories',
    'label' => 'Categories',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-feed_item-field_feed_item_content'.
  $field_instances['node-feed_item-field_feed_item_content'] = array(
    'bundle' => 'feed_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'advanced_text_formatter',
        'settings' => array(
          'allowed_html' => array(
            0 => 'a',
            1 => 'b',
            2 => 'br',
            3 => 'dd',
            4 => 'dl',
            5 => 'dt',
            6 => 'em',
            7 => 'i',
            8 => 'li',
            9 => 'ol',
            10 => 'p',
            11 => 'strong',
            12 => 'u',
            13 => 'ul',
          ),
          'autop' => 0,
          'ellipsis' => 1,
          'filter' => 'drupal',
          'format' => 'filtered_html',
          'token_replace' => 0,
          'trim_length' => 0,
          'use_summary' => 0,
          'word_boundary' => 1,
        ),
        'type' => 'advanced_text',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'advanced_text_formatter',
        'settings' => array(
          'allowed_html' => array(
            0 => 'a',
            1 => 'b',
            2 => 'br',
            3 => 'dd',
            4 => 'dl',
            5 => 'dt',
            6 => 'em',
            7 => 'i',
            8 => 'li',
            9 => 'ol',
            10 => 'p',
            11 => 'strong',
            12 => 'u',
            13 => 'ul',
          ),
          'autop' => 0,
          'ellipsis' => 1,
          'filter' => 'drupal',
          'format' => 'feed_item_teaser',
          'token_replace' => 0,
          'trim_length' => 200,
          'use_summary' => 0,
          'word_boundary' => 1,
        ),
        'type' => 'advanced_text',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_feed_item_content',
    'label' => 'Content',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
        'show_token_tree' => 0,
      ),
      'type' => 'text_textarea',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-feed_item-field_feed_item_keywords'.
  $field_instances['node-feed_item-field_feed_item_keywords'] = array(
    'bundle' => 'feed_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 12,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_feed_item_keywords',
    'label' => 'Keywords',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-feed_item-field_feed_item_stanbol_orgs'.
  $field_instances['node-feed_item-field_feed_item_stanbol_orgs'] = array(
    'bundle' => 'feed_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_feed_item_stanbol_orgs',
    'label' => 'Stanbol: Organizations',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-feed_item-field_feed_item_stanbol_people'.
  $field_instances['node-feed_item-field_feed_item_stanbol_people'] = array(
    'bundle' => 'feed_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_feed_item_stanbol_people',
    'label' => 'Stanbol: People',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-feed_item-field_feed_item_stanbol_places'.
  $field_instances['node-feed_item-field_feed_item_stanbol_places'] = array(
    'bundle' => 'feed_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'leaflet',
        'settings' => array(
          'height' => 400,
          'icon' => array(
            'html' => '',
            'htmlClass' => '',
            'iconAnchor' => array(
              'x' => '',
              'y' => '',
            ),
            'iconImageStyle' => '',
            'iconSize' => array(
              'x' => '',
              'y' => '',
            ),
            'iconType' => 'marker',
            'iconUrl' => '/profiles/newsfeeder/modules/contrib/ip_geoloc/markers/blue.png',
            'popupAnchor' => array(
              'x' => '',
              'y' => '',
            ),
            'shadowAnchor' => array(
              'x' => '',
              'y' => '',
            ),
            'shadowUrl' => '',
          ),
          'leaflet_map' => 'OSM Mapnik',
          'popup' => array(
            'show' => 0,
            'text' => '',
          ),
          'vector_display' => array(
            'clickable' => 0,
            'color' => '',
            'dashArray' => '',
            'fill' => 0,
            'fillColor' => '',
            'fillOpacity' => '',
            'opacity' => '',
            'stroke' => 0,
            'stroke_override' => 0,
            'weight' => '',
          ),
          'zoom' => array(
            'initialZoom' => -1,
            'maxZoom' => -1,
            'minZoom' => 1,
            'scrollWheelZoom' => 1,
          ),
        ),
        'type' => 'geofield_leaflet',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_feed_item_stanbol_places',
    'label' => 'Stanbol: Places',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'geofield',
      'settings' => array(
        'html5_geolocation' => 1,
      ),
      'type' => 'geofield_latlon',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-feed_item-field_feed_item_url'.
  $field_instances['node-feed_item-field_feed_item_url'] = array(
    'bundle' => 'feed_item',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_feed_item_url',
    'label' => 'Source',
    'required' => 1,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'configurable_class' => 0,
        'configurable_title' => 0,
        'rel' => 'nofollow',
        'target' => '_blank',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'rel_remove_internal',
      'title' => 'value',
      'title_label_use_field_label' => 0,
      'title_maxlength' => 128,
      'title_value' => '[node:title]',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Author(s)');
  t('Categories');
  t('Content');
  t('Keywords');
  t('Source');
  t('Stanbol: Organizations');
  t('Stanbol: People');
  t('Stanbol: Places');

  return $field_instances;
}
