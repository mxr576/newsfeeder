<?php

class views_handler_field_unpublished_feed_items_count extends views_handler_field_numeric {
  /**
   * {@inheritdoc}
   */
  function query() {
    // Query should not run, because the field is a
    // "virtual" field.
  }

  /**
   * {@inheritdoc}
   */
  function render($values) {
    // Get the number of unpublished feed items.
    $query = db_select('feeds_item', 'fi');
    $query->innerJoin('node', 'n', 'n.nid = fi.entity_id');
    $query->condition('n.status', NODE_NOT_PUBLISHED)
      ->condition('fi.feed_nid', $values->node_feeds_item_nid);
    return $query->countQuery()->execute()->fetchField();
  }
}
